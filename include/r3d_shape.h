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
struct Shape;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Vertex structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vertex
{
    Vector3 pos;
    Vector3 nrm;
    Vector2 uv;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// HitRecord structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct HitRecord
{
    float           dist    = F_HIT_MAX;                    // 衝突点までの距離.
    Vector3         pos     = Vector3(0.0f, 0.0f, 0.0f);    // 衝突点の位置座標.
    Vector3         nrm     = Vector3(0.0f, 0.0f, 0.0f);    // 法線ベクトル.
    Vector2         uv      = Vector2(0.0f, 0.0f);          // 衝突点のテクスチャ座標.
    const Shape*    shape   = nullptr;                      // 形状データ.
    const Material* mat     = nullptr;                      // 材質データ.
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Shape structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Shape
{
    virtual ~Shape() {}
    virtual bool hit(const Ray& ray, HitRecord& record) const = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Sphere sturcture
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Sphere : public Shape
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

    inline bool hit(const Ray& ray, HitRecord& record) const override
    {
        auto p = pos - ray.pos;
        auto b = dot(p, ray.dir);
        auto det = b * b - dot(p, p) + radius * radius;
        if (det < 0.0f)
        { return false; }

        auto sqrt_det = sqrt(det);
        auto t1 = b - sqrt_det;
        auto t2 = b + sqrt_det;
        if (t1 < F_HIT_MIN && t2 < F_HIT_MIN)
        { return false; }

        auto dist = ( t1 > F_HIT_MIN ) ? t1 : t2;
        if ( dist > record.dist )
        { return false; }

        record.dist  = dist;
        record.pos   = ray.pos + ray.dir * dist;
        record.nrm   = normalize(record.pos - pos);

        auto theta = acosf( record.nrm.y );
        auto phi   = atan2f( record.nrm.x, record.nrm.z );
        if ( phi < 0.0f )
        { phi += F_2PI; }

        record.uv    = Vector2( phi * F_1DIV2PI, ( F_PI - theta ) * F_1DIVPI );
        record.shape = this;
        record.mat   = mat;

        return true;
    }
};