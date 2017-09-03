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
#include <vector>
#include <algorithm>
#include <r3d_canvas.h>
#include <atomic>
#include <thread>
#include <cassert>
#ifdef _OPENMP
#include <omp.h>
#endif//_OPENMP

namespace {

//-------------------------------------------------------------------------------------------------
// Global Varaibles.
//-------------------------------------------------------------------------------------------------
const int     g_max_depth = 4;
const Vector3 g_back_ground (0.0,   0.0,    0.0);

const Material* g_basic_mat[] = {
    Lambert::create(Vector3(0.25f, 0.75f, 0.25f)),
    Lambert::create(Vector3(0.25f, 0.25f, 0.75f)),
    Lambert::create(Vector3(0.75f, 0.75f, 0.75f)),
    Lambert::create(Vector3(0.01f, 0.01f, 0.01f)),
    Mirror::create(Vector3(0.75f, 0.25f, 0.25f)),
    Lambert::create(Vector3(0.0f, 0.0f, 0.0f), Vector3(12.0f, 12.0f, 12.0f)),
    Refract::create(Vector3(0.99f, 0.99f, 0.99f), 1.5f)
};

const Sphere  g_spheres[] = {
    Sphere(1e5,     Vector3( 1e5 + 1.0,    40.8,          81.6), g_basic_mat[0]),
    Sphere(1e5,     Vector3(-1e5 + 99.0,   40.8,          81.6), g_basic_mat[1]),
    Sphere(1e5,     Vector3(50.0,          40.8,           1e5), g_basic_mat[2]),
    Sphere(1e5,     Vector3(50.0,          40.8,  -1e5 + 170.0), g_basic_mat[3]),
    Sphere(1e5,     Vector3(50.0,           1e5,          81.6), g_basic_mat[2]),
    Sphere(1e5,     Vector3(50.0,   -1e5 + 81.6,          81.6), g_basic_mat[2]),
    Sphere(16.5,    Vector3(27.0,          16.5,          47.0), g_basic_mat[4]),
    Sphere(16.5,    Vector3(73.0,          16.5,          78.0), g_basic_mat[6]),
    Sphere(5.0,     Vector3(50.0,          81.6,          81.6), g_basic_mat[5])
};
const int   g_lightId = 8;


//-------------------------------------------------------------------------------------------------
//      シーンとの交差判定を行います.
//-------------------------------------------------------------------------------------------------
inline bool intersect_scene(const Ray& ray, float* t, int* id)
{
    auto n = static_cast<int>(sizeof(g_spheres) / sizeof(g_spheres[0]));

    *t  = F_MAX;
    *id = -1;

    for (auto i = 0; i < n; ++i)
    {
        auto d = g_spheres[i].intersect(ray);
        if (d > F_HIT_MIN && d < *t)
        {
            *t  = d;
            *id = i;
        }
    }

    return (*t < F_HIT_MAX);
}

//-------------------------------------------------------------------------------------------------
//      放射輝度を求めます.
//-------------------------------------------------------------------------------------------------
Vector3 radiance(const Ray& input_ray, Random* random)
{
    Vector3 L(0, 0, 0);
    Vector3 W(1, 1, 1);
    Ray ray(input_ray.pos, input_ray.dir);

    auto direct_light = true;

    for(int depth=0; ; depth++)
    {
        float t;
        int   id;

        // シーンとの交差判定.
        if (!intersect_scene(ray, &t, &id))
        { break; }

        // 交差物体.
        const auto& obj = g_spheres[id];

        // 交差位置.
        const auto hit_pos = ray.pos + ray.dir * t;

        // 法線ベクトル.
        const auto normal  = normalize(hit_pos - obj.pos);

        // 物体からのレイの入出を考慮した法線ベクトル.
        const auto orienting_normal = (dot(normal, ray.dir) < 0.0) ? normal : -normal;

        auto p = obj.mat->threshold();

        if (direct_light)
        { L += W * obj.mat->emissive(); }

        direct_light = obj.mat->is_delta();

        // 打ち切り深度に達したら終わり.
        if(depth > g_max_depth)
        {
            if (random->get_as_float() >= p)
            { break; }
        }
        else
        {
            p = 1.0f;
        }

        ShadingArg arg = {};
        arg.input  = ray.dir;
        arg.normal = normal;
        arg.random = *random;

        // マテリアルの評価.
        auto w = obj.mat->shade(arg);

        // 直接光をサンプル.
        if (id != g_lightId && !obj.mat->is_delta())
        {
            const auto& light = g_spheres[g_lightId];

            const auto r1 = F_2PI * random->get_as_float();
            const auto r2 = 1.0f - 2.0f * random->get_as_float();
            const auto r3 = sqrt(1.0f - r2 * r2);
            const auto light_pos = light.pos + (light.radius + F_HIT_MIN) * normalize(Vector3(r3 * cos(r1), r3 * sin(r1), r2));

            // ライトベクトル.
            auto light_dir   = light_pos - hit_pos;

            // ライトへの距離の2乗
            auto light_dist2 = dot(light_dir, light_dir);

            // 正規化.
            light_dir = normalize(light_dir);

            // ライトの法線ベクトル.
            auto light_normal = normalize(light_pos - light.pos);

            auto dot0 = abs(dot(orienting_normal, light_dir));
            auto dot1 = abs(dot(light_normal,    -light_dir));
            auto rad2 = light.radius * light.radius;

            float  shadow_t;
            int    shadow_id;
            Ray    shadow_ray(hit_pos, light_dir);

            // シャドウレイを発射.
            auto hit = intersect_scene(shadow_ray, &shadow_t, &shadow_id);

            // ライトのみと衝突した場合のみ寄与を取る.
            {
                auto hit_light = hit && (shadow_id == g_lightId);
                if (hit_light)
                {
                    auto G = (dot0 * dot1) / light_dist2;

                    // ライトの確率密度.
                    auto light_pdf  = 1.0f / (4.0f * F_PI * rad2);
                    auto light_pdf2 = light_pdf * light_pdf;

                    // BRDFの確率密度.
                    auto brdf_pdf = arg.pdf / light_dist2;

                    // 多重重点的サンプルの重みを求める.
                    auto mis_weight = light_pdf2 / (light_pdf2 + brdf_pdf * brdf_pdf);

                    L += W * light.mat->emissive() * (w / F_PI) * G * mis_weight / light_pdf;
                }
            }
        }

        // レイを更新.
        ray = Ray(hit_pos, arg.output);

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
    #if 1
        // レンダーターゲットのサイズ.
        int width   = 1280;
        int height  = 960;
        int samples = 512;
    #else
        // レンダーターゲットのサイズ.
        int width   = 320;
        int height  = 240;
        int samples = 512;
    #endif

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
        printf_s("* width    : %d\n", width);
        printf_s("* height   : %d\n", height);
        printf_s("* samples  : %d\n", samples);

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

            // 4分33秒経ったら終了(レイトレ合宿5のルール準拠).
            if (sec >= 273)
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

    #ifdef _OPENMP
        // CPUコア数を取得.
        int core_count = omp_get_num_procs();

        // 1個は監視スレッド用に開けておく.
        if (core_count >= 2)
        { core_count--; }

        printf_s("* cpu core : %d\n", core_count);
    #endif//_OPENMP

    // カメラ用意.
    Camera camera(
        Vector3(50.0, 52.0, 295.6),
        normalize(Vector3(0.0, -0.042612, -1.0)),
        Vector3(0.0, 1.0, 0.0),
        0.5135,
        double(width) / double(height),
        130.0
    );

    // レンダーターゲット生成.
    canvas.resize(width, height);

    // 乱数初期化.
    Random random(123456);

    auto inv_w = 1.0 / double(width);
    auto inv_h = 1.0 / double(height);
    auto inv_s = 1.0 / double(samples);
    uint64_t ray_count = 0;

    for(auto s = 0; s < samples; ++s)
    {
    #ifdef _OPENMP
        #pragma omp parallel for schedule(dynamic, 1) num_threads(core_count)
    #endif//_OPENMP
        for (auto y = 0; y < height; ++y)
        for (auto x = 0; x < width;  ++x)
        {   
            auto fx = double(x) * inv_w - 0.5;
            auto fy = double(y) * inv_h - 0.5;

            // Let's レイトレ！
            canvas.add(x, y, radiance(camera.emit(fx, fy), &random) * inv_s);

            ray_count++;

            if (is_finish)
            { break; }
        }

        if (is_finish)
        {
            printf_s("rendering imcompleted...\n");
            printf_s("%.2lf%% complted.\n", (double(s) * inv_s * 100.0));
            break;
        }
    }

    printf_s("%llu ray generated.\n", ray_count);

    // 終了フラグを立てる.
    request_finish = true;

    // スレッドの終了を待機.
    thd.join();

    // 解放処理.
    for(auto& itr : g_basic_mat)
    {
        delete itr;
        itr = nullptr;
    }

    return 0;
}
