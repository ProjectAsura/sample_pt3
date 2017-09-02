//-------------------------------------------------------------------------------------------------
// File : r3d_material.h
// Desc : Material.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Material structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Material
{
    enum Type
    {
        None,
        Lambert,
        Mirror,
        Refract,
        Phong
    };

    Type            type;       //!< マテリアルタイプ.
    Vector3         albedo;     //!< 反射率.
    Vector3         emissive;   //!< エミッシブ.

    Material(Type t)
    : type      (t)
    , albedo    (Vector3(0.0f, 0.0f, 0.0f))
    , emissive  (Vector3(0.0f, 0.0f, 0.0f))
    { /* DO_NOTHING */ }

    Material(Type t, const Vector3& a)
    : type      (t)
    , albedo    (a)
    , emissive  (Vector3(0.0f, 0.0f, 0.0f))
    { /* DO_NOTHING */ }

    Material(Type t, const Vector3& a, const Vector3& e)
    : type      (t)
    , albedo    (a)
    , emissive  (e)
    { /* DO_NOTHING */ }
};


struct RefractMaterial : public Material
{
    float ior;      // 屈折率.

    RefractMaterial(const Vector3& a, float i)
    : Material  (Type::Refract, a)
    , ior       (i)
    { /* DO_NOTHING */ }

    RefractMaterial(const Vector3& a, float i, const Vector3& e)
    : Material  (Type::Refract, a, e)
    , ior       (i)
    { /* DO_NOTHING */ }
};

struct PhongMaterial : public Material
{
    float shininess;    // シャイネス.

    PhongMaterial(const Vector3& a, float s)
    : Material  (Type::Phong, a)
    , shininess (s)
    { /* DO_NOTHING */ }

    PhongMaterial(const Vector3& a, float s, const Vector3& e)
    : Material  (Type::Phong, a, e)
    , shininess (s)
    { /* DO_NOTHING */ }
};