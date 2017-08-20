﻿//-------------------------------------------------------------------------------------------------
// File : r3d_shape.h
// Desc : Shapes.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// ReflectionType enum
///////////////////////////////////////////////////////////////////////////////////////////////////
enum ReflectionType
{
    Lambert             = 0,    //!< Lamblert BRDF.
    PerfectSpecular     = 1,    //!< 完全鏡面反射.
    Refraction          = 2,    //!< 屈折.
    Phong               = 3,    //!< Phong BRDF
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Sphere sturcture
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Sphere
{
    float           radius;     //!< 半径です.
    Vector3         pos;        //!< 位置座標です.
    Vector3         color;      //!< 色です.
    ReflectionType  type;       //!< 反射タイプです.
    Vector3         emission;   //!< 自己照明成分.

    Sphere
    (
        float           r,
        const Vector3&  p,
        const Vector3&  c,
        ReflectionType  t,
        const Vector3&  e
    )
    : radius    (r)
    , pos       (p)
    , color     (c)
    , type      (t)
    , emission  (e)
    { /* DO_NOTHING*/ }

    inline float intersect(const Ray& ray) const
    {
        auto p = pos - ray.pos;
        auto b = dot(p, ray.dir);
        auto det = b * b - dot(p, p) + radius * radius;
        if (det >= 0.0f)
        {
            auto sqrt_det = sqrt(det);
            auto t1 = b - sqrt_det;
            auto t2 = b + sqrt_det;
            if (t1 > F_HIT_MIN)
            { return t1; }
            else if (t2 > F_HIT_MIN)
            { return t2; }
        }
 
        return F_HIT_MAX;
    }
};