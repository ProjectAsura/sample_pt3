//-------------------------------------------------------------------------------------------------
// File : r3d_camera.h
// Desc : Camera.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Camera class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
    Camera
    (
        const Vector3&  position,
        const Vector3&  dir,
        const Vector3&  upward,
        float           fov,
        float           znear,
        float           width,
        float           height
    )
    {
        pos         = position;
        axis_z      = normalize(dir);
        axis_x      = normalize(cross(axis_z, upward));
        axis_y      = normalize(cross(axis_z, axis_x));
        near_clip   = znear;

        auto aspect = width / height;
        inv_w = 1.0f / width;
        inv_h = 1.0f / height;

        auto tan_fov = tan(fov * 0.5f);
        axis_x *= tan_fov * aspect;
        axis_y *= tan_fov;
    }

    inline Ray emit(float x, float y) const
    {
        auto fx = x * inv_w - 0.5f;
        auto fy = y * inv_h - 0.5f;
        auto d = axis_x * fx + axis_y * fy + axis_z;
        auto p = pos + d * near_clip;
        return make_ray(p, normalize(d));
    }

private:
    Vector3 pos;        //!< 位置座標です.
    Vector3 axis_x;     //!< X軸
    Vector3 axis_y;     //!< Y軸
    Vector3 axis_z;     //!< Z軸
    float   inv_w;      //!< 横幅の逆数.
    float   inv_h;      //!< 縦幅の逆数.
    float   near_clip;  //!< スクリーンまでの距離.
};
