//-------------------------------------------------------------------------------------------------
// File : r3d_shape.h
// Desc : Shapes.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>


//-------------------------------------------------------------------------------------------------
// Forward Declaratiosn.
//-------------------------------------------------------------------------------------------------
struct Material;


///////////////////////////////////////////////////////////////////////////////////////////////////
// Sphere sturcture
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Sphere
{
    float           radius;     //!< 半径です.
    Vector3         pos;        //!< 位置座標です.
    const Material* mat;        //!< マテリアル.

    Sphere
    (
        float           r,
        const Vector3&  p,
        const Material* m
    )
    : radius    (r)
    , pos       (p)
    , mat       (m)    
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