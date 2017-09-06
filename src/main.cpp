//-------------------------------------------------------------------------------------------------
// File : main.cpp
// Desc : Main Entry Point.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Inlcudes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>
#include <r3d_camera.h>
#include <r3d_shape.h>
#include <r3d_material.h>
#include <r3d_scene.h>
#include <r3d_canvas.h>
#include <vector>
#include <algorithm>
#include <atomic>
#include <thread>
#include <cassert>
#include <direct.h>
#ifdef _OPENMP
#include <omp.h>
#endif//_OPENMP


namespace {

//-------------------------------------------------------------------------------------------------
// Global Varaibles.
//-------------------------------------------------------------------------------------------------
const int     g_max_depth = 3;
Scene         g_scene;


//-------------------------------------------------------------------------------------------------
//      放射輝度を求めます.
//-------------------------------------------------------------------------------------------------
Vector3 radiance(const Ray& input_ray, Random* random)
{
    Vector3 L(0, 0, 0);
    Vector3 W(1, 1, 1);
    Ray ray(input_ray.pos, input_ray.dir);

    auto direct_light = true;

    for(int depth=0;; depth++)
    {
        HitRecord record = {};

        if (!g_scene.hit(ray, record))
        {
            L += W * g_scene.sample_ibl(ray.dir);
            break;
        }

        auto p = record.mat->threshold();

        //if (direct_light)
        { L += W * record.mat->emissive(); }

        direct_light = record.mat->is_delta();

        // 打ち切り深度に達したら終わり.
        if(depth > g_max_depth)
        {
            if (random->get_as_float() >= p)
            {
                break;
            }
        }
        else
        {
            p = 1.0f;
        }

        ShadingArg arg = {};
        arg.input  = ray.dir;
        arg.normal = record.nrm;
        arg.random = *random;
        arg.uv     = record.uv;

        // マテリアルの評価.
        auto w = record.mat->shade(arg);

        //// 直接光をサンプル.
        //if (obj != g_spheres[g_lightId] && !record.mat->is_delta())
        //{
        //    Vector3 light_pos;
        //    Vector3 light_nrm;
        //    g_spheres[g_lightId]->sample(*random, light_pos, light_nrm);

        //    // ライトベクトル.
        //    auto light_dir = light_pos - hit_pos;

        //    // ライトへの距離の2乗
        //    auto light_dist2 = dot(light_dir, light_dir);

        //    // 正規化.
        //    light_dir = normalize(light_dir);

        //    ShadowRecord shadow_rec;
        //    Ray    shadow_ray(hit_pos, light_dir);

        //    // シャドウレイを発射.
        //    auto hit = intersect_scene(shadow_ray, shadow_rec);

        //    // ライトのみと衝突した場合のみ寄与を取る.
        //    {
        //        auto hit_light = hit && (shadow_rec.shape == g_spheres[g_lightId]);
        //        if (hit_light)
        //        {
        //            // 物体からのレイの入出を考慮した法線ベクトル.
        //            const auto orienting_normal = (dot(normal, ray.dir) < 0.0) ? normal : -normal;

        //            auto dot0 = abs(dot(orienting_normal, light_dir));
        //            auto dot1 = abs(dot(light_nrm,       -light_dir));

        //            auto G = (dot0 * dot1) / light_dist2;

        //            // ライトの確率密度.
        //            auto light_pdf2 = shadow_rec.pdf * shadow_rec.pdf;

        //            // BRDFの確率密度.
        //            auto brdf_pdf  = (arg.pdf / light_dist2);
        //            auto brdf_pdf2 = brdf_pdf * brdf_pdf;

        //            // 多重重点的サンプルの重みを求める.
        //            auto mis_weight = light_pdf2 / (light_pdf2 + brdf_pdf2);

        //            L += W * shadow_rec.mat->emissive() * (w / F_PI) * G * mis_weight / shadow_rec.pdf;
        //        }
        //    }
        //}

        // レイを更新.
        ray = Ray(record.pos, arg.output);

        // 重み更新.
        W *= (w / p);

        // 重みがゼロなら計算しても意味ないので打ち切り.
        if (is_zero(W))
        { break; }
    }

    return L;
}

} // namespace


//-------------------------------------------------------------------------------------------------
//      メインエントリーポイントです.
//-------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    Scene scn;
    scn.save("test_scene.xml");

    if (argc <= 1)
    {
        if (!g_scene.load("test_scene.xml"))
        {
            fprintf_s(stderr, "Error : Scene Load Failed.\n");
            return false;
        }
    }
    else
    {
        if (!g_scene.load(argv[1]))
        {
            fprintf_s(stderr, "Error : Scene Load Failed. file = %s\n", argv[1]);
            return false;
        }
    }

    int w = g_scene.width();
    int h = g_scene.height();
    int s = g_scene.samples();

    r3d::Canvas canvas;
    std::atomic<bool> is_finish      = false;   // 終了したかどうか?
    std::atomic<bool> request_finish = false;   // 終了要求フラグ.

    // 監視スレッド.
    std::thread thd([&]()
    {
        auto counter = 0;
        auto start   = std::chrono::system_clock::now();
        auto begin   = start;

        printf_s("start!\n");
        printf_s("* width    : %d\n", w);
        printf_s("* height   : %d\n", h);
        printf_s("* samples  : %d\n", s);

        while(!request_finish)
        {
            auto curr = std::chrono::system_clock::now();
            auto term = std::chrono::duration_cast<std::chrono::seconds>(curr - begin).count();
            auto sec  = std::chrono::duration_cast<std::chrono::seconds>(curr - start).count();

            // 30秒ごとにキャプチャー.
            if (term >= 30)
            {
                canvas.write(counter++);
                begin = curr;
            }

            // 4分32秒経ったら終了(レイトレ合宿5のルール準拠).
            // 終了処理等の時間があるので，1秒早めに終わらせる.
            if (sec >= 272)
            {
                break;
            }

            // 他に処理を渡すためにちょっと空ける.
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // 最後の1枚は出力する.
        canvas.write(counter++);

        {
            auto end = std::chrono::system_clock::now();
            auto sec = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

            printf_s("* time = %llu(sec)\n", sec);
        }

        is_finish = true;
        printf_s("end!\n");
    });

    // キャプチャディレクトリ作成.
    _mkdir("img");

    #ifdef _OPENMP
        // CPUコア数を取得.
        int core_count = omp_get_num_procs();

        // 1個は監視スレッド用に開けておく.
        if (core_count >= 2)
        { core_count--; }

        printf_s("* cpu core : %d\n", core_count);
    #endif//_OPENMP

    // レンダーターゲット生成.
    canvas.resize(w, h);

    // 乱数初期化.
    Random random(123456);

    auto inv_s = 1.0f / float(s);
    uint64_t ray_count = 0;

    for(auto loop = 0; loop < s; ++loop)
    {
    #ifdef _OPENMP
        #pragma omp parallel for schedule(dynamic, 1) num_threads(core_count)
    #endif//_OPENMP
        for (auto y = 0; y < h; ++y)
        for (auto x = 0; x < w; ++x)
        {   
            // Let's レイトレ！
            canvas.add(x, y, radiance(g_scene.emit(float(x), float(y)), &random) * inv_s);

            ray_count++;

            if (is_finish)
            { break; }
        }

        if (is_finish)
        {
            printf_s("rendering imcompleted...\n");
            printf_s("%.2f%% complted.\n", (loop * inv_s * 100.0f));
            break;
        }
    }

    printf_s("%llu ray generated.\n", ray_count);

    // 終了フラグを立てる.
    request_finish = true;

    // スレッドの終了を待機.
    thd.join();

    return 0;
}
