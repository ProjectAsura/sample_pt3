//-------------------------------------------------------------------------------------------------
// File : r3d_canvas.cpp
// Desc : Canvas Module.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_canvas.h>
#include <algorithm>
#include <stb_image_write.h>


namespace {

//------------------------------------------------------------------------------------------------
//      輝度値を取得します.
//------------------------------------------------------------------------------------------------
float RGBToY(const Vector3& value)
{
    // cf. https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.601-7-201103-I!!PDF-E.pdf
    const Vector3 bt601_to_luminance(0.299f, 0.587f, 0.114f);
    return dot(value, bt601_to_luminance);
}

//------------------------------------------------------------------------------------------------
//      対数平均と最大輝度値を求めます.
//------------------------------------------------------------------------------------------------
void CalcLogAve
(
    int             width,
    int             height,
    const Vector3*  pPixels,
    float           epsilon,
    float&          aveLw,
    float&          maxLw
)
{
    auto init = false;
    for( auto i=0; i<height; ++i )
    {
        for( auto j=0; j<width; ++j )
        {
            // ピクセル番号.
            auto idx = ( i * width ) + j;

            // 輝度値に変換.
            auto Lw = RGBToY( pPixels[idx] );

            // 最大輝度値を求める.
            if ( !init )
            {
                maxLw = Lw;
                init = true;
            }
            else
            { maxLw = max( maxLw, Lw ); }

            // 輝度値の対数総和を求める.
            aveLw += logf( epsilon + Lw );
        }
    }

    // ピクセル数で除算.
    aveLw /= ( width * height );

    // 指数をとる.
    aveLw = expf( aveLw );
}

} // namespace 


namespace r3d {

///////////////////////////////////////////////////////////////////////////////////////////////////
// Canvas class
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//      コンストラクタです.
//-------------------------------------------------------------------------------------------------
Canvas::Canvas()
{ /* DO_NOTHING */ }

//-------------------------------------------------------------------------------------------------
//      デストラクタです.
//-------------------------------------------------------------------------------------------------
Canvas::~Canvas()
{ m_pixels.clear(); }

//-------------------------------------------------------------------------------------------------
//      レンダーターゲットのサイズを設定します.
//-------------------------------------------------------------------------------------------------
void Canvas::resize(int w, int h)
{
    m_w = w;
    m_h = h;
    auto count = m_w * m_h;
    m_pixels.resize(count);
    m_temps.resize(count);
    m_output.resize(count * 3);

    for(auto i=0; i<count; ++i)
    { m_pixels[i] = Vector3(0.0f, 0.0f, 0.0f); }
}

//-------------------------------------------------------------------------------------------------
//      指定ピクセルを取得します.
//-------------------------------------------------------------------------------------------------
Vector3& Canvas::at(int x, int y)
{ return m_pixels[y * m_w + x]; }

//-------------------------------------------------------------------------------------------------
//      指定ピクセルを取得します(const版).
//-------------------------------------------------------------------------------------------------
const Vector3& Canvas::at(int x, int y) const
{ return m_pixels[y * m_w + x]; }

//-------------------------------------------------------------------------------------------------
//      ピクセルを取得します.
//-------------------------------------------------------------------------------------------------
const Vector3* Canvas::data() const
{ return m_pixels.data(); }

//-------------------------------------------------------------------------------------------------
//      ピクセルに色を加算します.
//-------------------------------------------------------------------------------------------------
void Canvas::add(int x, int y, const Vector3& value)
{ m_pixels[y * m_w + x] += value; }

//-------------------------------------------------------------------------------------------------
//      ファイルに書き出します.
//-------------------------------------------------------------------------------------------------
bool Canvas::write(const char* filename)
{
    tonemap_aces();
    srgb_correction();

    for (size_t i = 0; i < m_temps.size(); ++i)
    {
        m_output[i * 3 + 0] = static_cast<uint8_t>(saturate(m_temps[i].x) * 255.0f + 0.5f);
        m_output[i * 3 + 1] = static_cast<uint8_t>(saturate(m_temps[i].y) * 255.0f + 0.5f);
        m_output[i * 3 + 2] = static_cast<uint8_t>(saturate(m_temps[i].z) * 255.0f + 0.5f);
    }

    return stbi_write_bmp(filename, m_w, m_h, 3, m_output.data()) == 1;
}

//-------------------------------------------------------------------------------------------------
//      レイトレ合宿用にファイルに書き出します.
//-------------------------------------------------------------------------------------------------
bool Canvas::write(int counter)
{
    char path[256] = {};
    sprintf_s(path, "img/%03d.bmp", counter);
    if (write(path))
    {
        printf_s("captured. %s\n", path);
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
//      トーンマップを適用しない.
//-------------------------------------------------------------------------------------------------
void Canvas::tonemap_none()
{
    for (size_t i = 0; i < m_pixels.size(); ++i)
    {
        m_temps[i] = m_pixels[i];
    }
}

//-------------------------------------------------------------------------------------------------
//      Reinhardトーンマッピングを適用します.
//-------------------------------------------------------------------------------------------------
void Canvas::tonemap_reinhard()
{
    auto a     = 0.18f;
    auto aveLw = 0.0f;
    auto maxLw = 0.0f;

    // 対数平均と最大輝度値を求める.
    CalcLogAve( m_w, m_h, m_pixels.data(), 0.00001f, aveLw, maxLw );

    auto coeff = a / aveLw;
    auto maxLw2 = maxLw * coeff;
    maxLw2 *= maxLw2;

    for(size_t i=0; i<m_temps.size(); ++i)
    {
        auto l = m_pixels[i] * coeff;
        m_temps[i].x = l.x * (1.0f + (l.x / maxLw2)) / (1.0f + l.x);
        m_temps[i].y = l.y * (1.0f + (l.y / maxLw2)) / (1.0f + l.y);
        m_temps[i].z = l.z * (1.0f + (l.z / maxLw2)) / (1.0f + l.z);
    }
}

//------------------------------------------------------------------------------------------------
//      ACES Flimicトーンマッピングを適用します.
//-------------------------------------------------------------------------------------------------
void Canvas::tonemap_aces()
{
    auto a_    = 0.18f;
    auto aveLw = 0.0f;
    auto maxLw = 0.0f;

    // 対数平均と最大輝度値を求める.
    CalcLogAve( m_w, m_h, m_pixels.data(), 0.00001f, aveLw, maxLw );

    auto coeff = a_ / aveLw;
    auto maxLw2 = maxLw * coeff;
    maxLw2 *= maxLw2;

    auto a = 2.51f;
    auto b = 0.03f;
    auto c = 2.43f;
    auto d = 0.59f;
    auto e = 0.14f;

    for(size_t i=0; i<m_temps.size(); ++i)
    {
        auto p = m_pixels[i] * coeff * 0.6f;

        m_temps[i].x = saturate((p.x * (a * p.x + b)) / (p.x * (c * p.x + d) + e));
        m_temps[i].y = saturate((p.y * (a * p.y + b)) / (p.y * (c * p.y + d) + e));
        m_temps[i].z = saturate((p.z * (a * p.z + b)) / (p.z * (c * p.z + d) + e));
    }
}

//-------------------------------------------------------------------------------------------------
//      ガンマ補正を適用します.
//-------------------------------------------------------------------------------------------------
void Canvas::gamma_correction()
{
    for(size_t i=0; i<m_temps.size(); ++i)
    {
        auto value = m_temps[i];
        m_temps[i].x = pow(value.x, 1.0f / 2.2f);
        m_temps[i].y = pow(value.y, 1.0f / 2.2f);
        m_temps[i].z = pow(value.z, 1.0f / 2.2f);
    }
}

//-------------------------------------------------------------------------------------------------
//      sRGB変換を適用します.
//-------------------------------------------------------------------------------------------------
void Canvas::srgb_correction()
{
    for(size_t i=0; i<m_temps.size(); ++i)
    {
        auto value = m_temps[i];

        if (value.x < 0.0031308f)
        { m_temps[i].x = 12.92f * value.x; }
        else
        { m_temps[i].x = (1.0f + 0.055f) * pow(value.x, 1.0f / 2.4f) - 0.055f; }

        if (value.y < 0.0031308f)
        { m_temps[i].y = 12.92f * value.y; }
        else
        { m_temps[i].y = (1.0f + 0.055f) * pow(value.y, 1.0f / 2.4f) - 0.055f; }

        if (value.z < 0.0031308f)
        { m_temps[i].z = 12.92f * value.z; }
        else
        { m_temps[i].z = (1.0f + 0.055f) * pow(value.z, 1.0f / 2.4f) - 0.055f; }
    }
}

//-------------------------------------------------------------------------------------------------
//      中央値フィルタを適用します.
//-------------------------------------------------------------------------------------------------
void Canvas::median_filter()
{
    struct item
    {
        double luminance;
        int    array_index;

        item()
        { /* DO_NOTHING */ }

        item(double lum, int index)
            : luminance(lum)
            , array_index(index)
        { /* DO_NOTHING */ }

        bool operator < (const item& value)
        { return luminance < value.luminance; }
    };

    for (auto y = 0; y < m_h; ++y)
    {
        for (auto x = 0; x < m_w; ++x)
        {
            Vector3 px[9] = {};
            px[4] = m_temps[y * m_w + x]; // 中央
            px[0] = (x == 0 || y == 0)                  ? px[4] : m_temps[(y - 1) * m_w + (x - 1)];     // 左上
            px[1] = (y == 0)                            ? px[4] : m_temps[(y - 1) * m_w + (x + 0)];     // 上
            px[2] = (x == (m_w - 1) || y == 0)          ? px[4] : m_temps[(y - 1) * m_w + (x + 1)];     // 右上
            px[3] = (x == 0)                            ? px[4] : m_temps[(y + 0) * m_w + (x - 1)];     // 左
            px[5] = (x == (m_w - 1))                    ? px[4] : m_temps[(y + 0) * m_w + (x + 1)];     // 右
            px[6] = (x == 0 || y == (m_h - 1))          ? px[4] : m_temps[(y + 1) * m_w + (x - 1)];     // 左下
            px[7] = (y == (m_h - 1))                    ? px[4] : m_temps[(y + 1) * m_w + (x + 0)];     // 下
            px[8] = (x == (m_w - 1) || y == (m_h - 1))  ? px[4] : m_temps[(y + 1) * m_w + (x + 1)];     // 右下

            // 輝度値を求める.
            item lum[9];
            lum[0] = item(RGBToY(px[0]), 0);
            lum[1] = item(RGBToY(px[1]), 1);
            lum[2] = item(RGBToY(px[2]), 2);
            lum[3] = item(RGBToY(px[3]), 3);
            lum[4] = item(RGBToY(px[4]), 4);
            lum[5] = item(RGBToY(px[5]), 5);
            lum[6] = item(RGBToY(px[6]), 6);
            lum[7] = item(RGBToY(px[7]), 7);
            lum[8] = item(RGBToY(px[8]), 8);

            // 並び替え.
            std::sort(std::begin(lum), std::end(lum));

            // 中央の値を採用.
            m_temps[y * m_w + x] = px[lum[4].array_index];
        }
    }
}

} // namespace s3d
