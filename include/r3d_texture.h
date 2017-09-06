//-------------------------------------------------------------------------------------------------
// File : r3d_texture.h
// Desc : Texture
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once


//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Texture class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Texture
{
public:
    Texture();
    ~Texture();

    bool load(const char* filename);

    Vector3 sample2d(const Vector2& texcoord) const;
    Vector3 sample3d(const Vector3& texcoord) const;

private:
    int     m_w     = 0;
    int     m_h     = 0;
    int     m_c     = 0;
    float*  m_buf   = nullptr;

    Vector3 at(int x, int y) const;
    Vector3 sample_point   (const Vector2& texcoord) const;
    Vector3 sample_bilinear(const Vector2& texcoord) const;
};