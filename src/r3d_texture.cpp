﻿//-------------------------------------------------------------------------------------------------
// File : r3d_texture.cpp
// Desc : Texture
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_texture.h>
#include <cassert>
#include <stb_image.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Texture class
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//      コンストラクタです.
//-------------------------------------------------------------------------------------------------
Texture::Texture()
{ /* DO_NOTHING */ }

//-------------------------------------------------------------------------------------------------
//      デストラクタです.
//-------------------------------------------------------------------------------------------------
Texture::~Texture()
{
    if (m_buf != nullptr)
    {
        stbi_image_free(m_buf);
        m_buf = nullptr;
    }

    m_w = 0;
    m_h = 0;
    m_c = 0;
}

//-------------------------------------------------------------------------------------------------
//      読み込み処理.
//-------------------------------------------------------------------------------------------------
bool Texture::load(const char* filename)
{
    m_buf = stbi_loadf(filename, &m_w, &m_h, &m_c, 3);
    return m_buf != nullptr;
}

//-------------------------------------------------------------------------------------------------
//      2次元テクスチャとしてフェッチ.
//-------------------------------------------------------------------------------------------------
Vector3 Texture::sample2d(const Vector2& texcoord) const
{ return sample_bilinear(texcoord); }

//-------------------------------------------------------------------------------------------------
//      スフィアマップとしてフェッチ.
//-------------------------------------------------------------------------------------------------
Vector3 Texture::sample3d(const Vector3& texcoord) const
{
    Vector2 uv( 0.0f, acos(texcoord.y) / F_PI);

    if (!is_zero(texcoord.x) || !is_zero(texcoord.z))
    {
        auto phi = atan2(texcoord.z, texcoord.x);
        if (texcoord.z < 0.0f)
        { phi += F_2PI; }

        uv.x = phi / F_2PI;
    }

    return sample_bilinear(uv);
}

//-------------------------------------------------------------------------------------------------
//      指定ピクセルを取得します.
//-------------------------------------------------------------------------------------------------
Vector3 Texture::at(int x, int y) const
{
    x = abs(x % m_w);
    y = abs(y % m_h);

    auto idx = m_w * 3 * y + x * 3;
    return Vector3(m_buf[idx + 0], m_buf[idx + 1], m_buf[idx + 2]);
}

//-------------------------------------------------------------------------------------------------
//      ポイントサンプリングを行います.
//-------------------------------------------------------------------------------------------------
Vector3 Texture::sample_point(const Vector2& texcoord) const
{
    auto x = int(texcoord.x * m_w + 0.5f);
    auto y = int(texcoord.y * m_h + 0.5f);
    return at(x, y);
}

//-------------------------------------------------------------------------------------------------
//      バイリニアサンプリングを行います.
//-------------------------------------------------------------------------------------------------
Vector3 Texture::sample_bilinear(const Vector2& texcoord) const
{
    auto fx = texcoord.x * m_w;
    auto fy = texcoord.y * m_h;

    auto x0 = int(floor(fx));
    auto y0 = int(floor(fy));

    auto x1 = x0 + 1;
    auto y1 = y0 + 1;

    return (x1 - fx) * ((y1 - fy) * at(x0, y0) + (fy - y0) * at(x0, y1))
         + (fx - x0) * ((y1 - fy) * at(x1, y0) + (fy - y0) * at(x1, y1));
}