//-------------------------------------------------------------------------------------------------
// File : r3d_math.h
// Desc : Math 
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

#define NOMINMAX

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <cstdint>
#include <cmath>


//-------------------------------------------------------------------------------------------------
// Constant Values
//-------------------------------------------------------------------------------------------------
constexpr float   F_HIT_MAX   = 1e12f;                                  //!< 交差判定上限値.
constexpr float   F_HIT_MIN   = 1e-1f;                                  //!< 交差判定下限値.
constexpr float   F_PI        = 3.1415926535897932384626433832795f;     //!< πです.
constexpr float   F_2PI       = 6.283185307179586476925286766559f;      //!< 2πです.
constexpr float   F_1DIVPI    = 0.31830988618379067153776752674503f;    //!< 1/πです.
constexpr float   F_1DIV2PI   = 0.15915494309189533576888376337251f;    //!< 1/2πです.
constexpr float   F_PIDIV2    = 1.5707963267948966192313216916398f;     //!< π/2です.
constexpr float   F_PIDIV3    = 1.0471975511965977461542144610932f;     //!< π/3です.
constexpr float   F_PIDIV4    = 0.78539816339744830961566084581988f;    //!< π/4です.
constexpr float   F_MAX       = 3.402823466e+38F;                       //!< float型の最大値です.
constexpr float   F_MIN       = 1.175494351e-38F;                       //!< float型の最小値です.
constexpr float   F_EPSILON   = 1.192092896e-07F;                       //!< マシンイプシロン.

inline float max(float lhs, float rhs)
{ return (lhs > rhs) ? lhs : rhs; }

inline float min(float lhs, float rhs)
{ return (lhs < rhs) ? lhs : rhs; }

inline float clamp(float value, float mini, float maxi)
{ return max(mini, min(maxi, value)); }

inline float saturate(float value)
{ return clamp(value, 0, 1); }

inline float lerp(float a, float b, float t)
{ return a + (b - a) * t; }

inline bool is_nan(float value)
{ return value != value; }

inline bool is_inf(float value)
{ return std::isinf(value); }

inline bool is_zero(float value)
{ return fabs(value) < F_EPSILON; }

inline float radian(float deg)
{ return deg * (F_PI / 180.0f); }

inline float degree(float rad)
{ return rad * (180.0f / F_PI); }


///////////////////////////////////////////////////////////////////////////////////////////////////
// Vector2 structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector2
{
    float x;
    float y;

    Vector2()
    { /* DO_NOTHING */ }

    Vector2(float nx, float ny)
    : x(nx), y(ny)
    { /* DO_NOTHING */ }

    inline Vector2 operator + () const
    { return *this; }

    inline Vector2 operator - () const
    { return Vector2(-x, -y); }

    inline Vector2 operator + (const Vector2& value) const
    { return Vector2(x + value.x, y + value.y); }

    inline Vector2 operator - (const Vector2& value) const
    { return Vector2(x - value.x, y - value.y); }

    inline Vector2 operator * (const Vector2& value) const
    { return Vector2(x * value.x, y * value.y); }

    inline Vector2 operator * (float value) const
    { return Vector2(x * value, y * value); }

    inline Vector2 operator / (float value) const
    { return Vector2(x / value, y / value); }

    inline Vector2& operator += (const Vector2& value)
    { x += value.x; y += value.y; return *this; }

    inline Vector2& operator -= (const Vector2& value)
    { x -= value.x; y -= value.y; return *this; }

    inline Vector2& operator *= (const Vector2& value)
    { x *= value.x; y *= value.y; return *this; }

    inline Vector2& operator *= (float value)
    { x *= value; y *= value; return *this; }

    inline Vector2& operator /= (float value)
    { x /= value; y /= value; return *this; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Vector3 structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector3
{
    float x;
    float y;
    float z;

    Vector3()
    { /* DO_NOTHING */ }

    Vector3(float nx, float ny, float nz)
    : x(nx), y(ny), z(nz)
    { /* DO_NOTHING */ }

    inline Vector3 operator + () const
    { return *this; }

    inline Vector3 operator - () const
    { return Vector3(-x, -y, -z); }

    inline Vector3 operator + (const Vector3& value) const
    { return Vector3(x + value.x, y + value.y, z + value.z); }

    inline Vector3 operator - (const Vector3& value) const
    { return Vector3(x - value.x, y - value.y, z - value.z); }

    inline Vector3 operator * (const Vector3& value) const
    { return Vector3(x * value.x, y * value.y, z * value.z); }

    inline Vector3 operator * (float value) const
    { return Vector3(x * value, y * value, z * value); }

    inline Vector3 operator / (const Vector3& value) const
    { return Vector3(x / value.x, y / value.y, z / value.z); }

    inline Vector3 operator / (float value) const
    { return Vector3(x / value, y / value, z / value); }

    inline Vector3& operator += (const Vector3& value)
    { x += value.x; y += value.y; z += value.z; return *this; }

    inline Vector3& operator -= (const Vector3& value)
    { x -= value.x; y -= value.y; z -= value.z; return *this; }

    inline Vector3& operator *= (const Vector3& value)
    { x *= value.x; y *= value.y; z *= value.z; return *this; }

    inline Vector3& operator *= (float value)
    { x *= value; y *= value; z *= value; return *this; }

    inline Vector3& operator /= (float value)
    { x /= value; y /= value; z /= value; return *this; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Ray structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Ray
{
    Vector3 pos;
    Vector3 dir;

    Ray(const Vector3& p, const Vector3& d)
    : pos(p), dir(d)
    { /* DO_NOTHING */ }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Random class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Random
{
public:
    Random()
    { set_seed( 123456789 ); }

    Random(uint32_t seed)
    { set_seed(seed); }

    inline void set_seed(uint32_t seed)
    {
        a = 123456789;
        b = 362436069;
        c = 521288629;
        d = ( seed <= 0 ) ? 88675123 : seed;
    }

    inline uint32_t get()
    {
        auto t = a ^ (a << 11);
        a = b;
        b = c;
        c = d;
        d = (d ^ (d >> 19)) ^ (t ^ (t >> 8));
        return d;
    }

    inline float get_as_float()
    { return static_cast<float>(get()) /  0xffffffffui32; }

private:
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Matrix structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Matrix
{
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;

    Matrix()
    { /* DO_NOTHING */ }

    Matrix(
        float m11, float m12, float m13, float m14,
        float m21, float m22, float m23, float m24,
        float m31, float m32, float m33, float m34,
        float m41, float m42, float m43, float m44)
    : _11(m11), _12(m12), _13(m13), _14(m14)
    , _21(m21), _22(m22), _23(m23), _24(m24)
    , _31(m31), _32(m32), _33(m33), _34(m34)
    , _41(m41), _42(m42), _43(m43), _44(m44)
    { /* DO_NOTHING */ }

    inline Matrix operator * (const Matrix& value) const
    {
        return Matrix(
            ( _11 * value._11 ) + ( _12 * value._21 ) + ( _13 * value._31 ) + ( _14 * value._41 ),
            ( _11 * value._12 ) + ( _12 * value._22 ) + ( _13 * value._32 ) + ( _14 * value._42 ),
            ( _11 * value._13 ) + ( _12 * value._23 ) + ( _13 * value._33 ) + ( _14 * value._43 ),
            ( _11 * value._14 ) + ( _12 * value._24 ) + ( _13 * value._34 ) + ( _14 * value._44 ),

            ( _21 * value._11 ) + ( _22 * value._21 ) + ( _23 * value._31 ) + ( _24 * value._41 ),
            ( _21 * value._12 ) + ( _22 * value._22 ) + ( _23 * value._32 ) + ( _24 * value._42 ),
            ( _21 * value._13 ) + ( _22 * value._23 ) + ( _23 * value._33 ) + ( _24 * value._43 ),
            ( _21 * value._14 ) + ( _22 * value._24 ) + ( _23 * value._34 ) + ( _24 * value._44 ),

            ( _31 * value._11 ) + ( _32 * value._21 ) + ( _33 * value._31 ) + ( _34 * value._41 ),
            ( _31 * value._12 ) + ( _32 * value._22 ) + ( _33 * value._32 ) + ( _34 * value._42 ),
            ( _31 * value._13 ) + ( _32 * value._23 ) + ( _33 * value._33 ) + ( _34 * value._43 ),
            ( _31 * value._14 ) + ( _32 * value._24 ) + ( _33 * value._34 ) + ( _34 * value._44 ),

            ( _41 * value._11 ) + ( _42 * value._21 ) + ( _43 * value._31 ) + ( _44 * value._41 ),
            ( _41 * value._12 ) + ( _42 * value._22 ) + ( _43 * value._32 ) + ( _44 * value._42 ),
            ( _41 * value._13 ) + ( _42 * value._23 ) + ( _43 * value._33 ) + ( _44 * value._43 ),
            ( _41 * value._14 ) + ( _42 * value._24 ) + ( _43 * value._34 ) + ( _44 * value._44 )
        );
    }

    inline Matrix operator * (float value) const
    {
        return Matrix(
            _11 * value, _12 * value, _13 * value, _14 * value,
            _21 * value, _22 * value, _23 * value, _24 * value,
            _31 * value, _32 * value, _33 * value, _34 * value,
            _41 * value, _42 * value, _43 * value, _44 * value);
    }

    static Matrix create_identiy()
    {
        return Matrix(
            1.0f,   0.0f,   0.0f,   0.0f,
            0.0f,   1.0f,   0.0f,   0.0f,
            0.0f,   0.0f,   1.0f,   0.0f,
            0.0f,   0.0f,   0.0f,   1.0f);
    }

    static Matrix create_scale(float x, float y, float z)
    {
        return Matrix(
            x,      0.0f,   0.0f,   0.0f,
            0.0f,   y,      0.0f,   0.0f,
            0.0f,   0.0f,   z,      0.0f,
            0.0f,   0.0f,   0.0f,   1.0f);
    }

    static Matrix create_translation(float x, float y, float z)
    {
        return Matrix(
            1.0f,   0.0f,   0.0f,   0.0f,
            0.0f,   1.0f,   0.0f,   0.0f,
            0.0f,   0.0f,   1.0f,   0.0f,
            x,      y,      z,      1.0f);
    }

    static Matrix create_rotate_x(float rad)
    {
        auto cosRad = cosf(rad);
        auto sinRad = sinf(rad);
        return Matrix(
            1.0f,   0.0f,   0.0f,   0.0f,
            0.0f,   cosRad, sinRad, 0.0f,
            0.0f,  -sinRad, cosRad, 0.0f,
            0.0f,   0.0f,   0.0f,   1.0f );
    }

    static Matrix create_rotate_y(float rad)
    {
        auto cosRad = cosf(rad);
        auto sinRad = sinf(rad);

        return Matrix(
            cosRad, 0.0f,  -sinRad, 0.0f,
            0.0f,   1.0f,   0.0f,   0.0f,
            sinRad, 0.0f,   cosRad, 0.0f,
            0.0f,   0.0f,   0.0f,   1.0f );
    }

    static Matrix create_rotate_z(float rad)
    {
        auto cosRad = cosf(rad);
        auto sinRad = sinf(rad);

        return Matrix( 
            cosRad, sinRad, 0.0f, 0.0f,
           -sinRad, cosRad, 0.0f, 0.0f,
            0.0f,   0.0f,   1.0f, 0.0f,
            0.0f,   0.0f,   0.0f, 1.0f );
    }
};

inline Vector2 operator * (float lhs, const Vector2& rhs)
{ return Vector2(lhs * rhs.x, lhs * rhs.y); }

inline Vector3 operator * (float lhs, const Vector3& rhs)
{ return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }

inline float length(const Vector2& value)
{ return sqrt(value.x * value.x + value.y * value.y); }

inline float length(const Vector3& value)
{ return sqrt(value.x * value.x + value.y * value.y + value.z * value.z); }

inline Vector2 saturate(const Vector2& value)
{
    return Vector2(
        saturate(value.x),
        saturate(value.y)
    );
}

inline Vector3 saturate(const Vector3& value)
{
    return Vector3(
        saturate(value.x),
        saturate(value.y),
        saturate(value.z)
    );
}

inline Vector2 lerp(const Vector2& a, const Vector2& b, float t)
{
    return Vector2(
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t)
    );
}

inline Vector3 lerp(const Vector3& a, const Vector3& b, float t)
{
    return Vector3(
        lerp(a.x, b.x, t),
        lerp(a.y, b.y, t),
        lerp(a.z, b.z, t)
    );
}

inline bool is_zero(const Vector2& value)
{ return is_zero(value.x) && is_zero(value.y); }

inline bool is_zero(const Vector3& value)
{ return is_zero(value.x) && is_zero(value.y) && is_zero(value.z); }

inline bool is_nan(const Vector2& value)
{ return is_nan(value.x) || is_nan(value.y); }

inline bool is_nan(const Vector3& value)
{ return is_nan(value.x) || is_nan(value.y) || is_nan(value.z); }

inline bool is_inf(const Vector2& value)
{ return is_inf(value.x) || is_inf(value.y); }

inline bool is_inf(const Vector3& value)
{ return is_inf(value.x) || is_inf(value.y) || is_inf(value.z); }

inline float dot(const Vector2& lhs, const Vector2& rhs)
{ return lhs.x * rhs.x + lhs.y * rhs.y; }

inline float dot(const Vector3& lhs, const Vector3& rhs)
{ return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

inline Vector2 normalize(const Vector2& value)
{ return value / length(value); }

inline Vector3 normalize(const Vector3& value)
{ return value / length(value); }

inline Vector2 reflect(const Vector2& i, const Vector2& n)
{ return i - n * 2.0f * dot(i, n); }

inline Vector3 reflect(const Vector3& i, const Vector3& n)
{ return i - n * 2.0f * dot(i, n); }

inline Vector3 cross(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(
        (lhs.y * rhs.z) - (lhs.z * rhs.y),
        (lhs.z * rhs.x) - (lhs.x * rhs.z),
        (lhs.x * rhs.y) - (lhs.y * rhs.x));
}

inline Matrix transpose(const Matrix& value)
{
    return Matrix(
        value._11, value._21, value._31, value._41,
        value._12, value._22, value._32, value._42,
        value._13, value._23, value._33, value._43,
        value._14, value._24, value._34, value._44);
}

inline float determinant(const Matrix& value)
{
    return
        value._11 * value._22 * value._33 * value._44 + value._11 * value._23 * value._34 * value._42 +
        value._11 * value._24 * value._32 * value._43 + value._12 * value._21 * value._34 * value._43 +
        value._12 * value._23 * value._31 * value._44 + value._12 * value._24 * value._33 * value._41 +
        value._13 * value._21 * value._32 * value._44 + value._13 * value._22 * value._34 * value._41 +
        value._13 * value._24 * value._31 * value._42 + value._14 * value._21 * value._33 * value._42 +
        value._14 * value._22 * value._31 * value._43 + value._14 * value._23 * value._32 * value._41 -
        value._11 * value._22 * value._34 * value._43 - value._11 * value._23 * value._32 * value._44 -
        value._11 * value._24 * value._33 * value._42 - value._12 * value._21 * value._33 * value._44 -
        value._12 * value._23 * value._34 * value._41 - value._12 * value._24 * value._31 * value._43 -
        value._13 * value._21 * value._34 * value._42 - value._13 * value._22 * value._31 * value._44 -
        value._13 * value._24 * value._32 * value._41 - value._14 * value._21 * value._32 * value._43 -
        value._14 * value._22 * value._33 * value._41 - value._14 * value._23 * value._31 * value._42;
}

inline Matrix invert(const Matrix& value)
{
    auto det = determinant(value);

    auto m11 = value._22*value._33*value._44 + value._23*value._34*value._42 + value._24*value._32*value._43 - value._22*value._34*value._43 - value._23*value._32*value._44 - value._24*value._33*value._42;
    auto m12 = value._12*value._34*value._43 + value._13*value._32*value._44 + value._14*value._33*value._42 - value._12*value._33*value._44 - value._13*value._34*value._42 - value._14*value._32*value._43;
    auto m13 = value._12*value._23*value._44 + value._13*value._24*value._42 + value._14*value._22*value._43 - value._12*value._24*value._43 - value._13*value._22*value._44 - value._14*value._23*value._42;
    auto m14 = value._12*value._24*value._33 + value._13*value._22*value._34 + value._14*value._23*value._32 - value._12*value._23*value._34 - value._13*value._24*value._32 - value._14*value._22*value._33;

    auto m21 = value._21*value._34*value._43 + value._23*value._31*value._44 + value._24*value._33*value._41 - value._21*value._33*value._44 - value._23*value._34*value._41 - value._24*value._31*value._43;
    auto m22 = value._11*value._33*value._44 + value._13*value._34*value._41 + value._14*value._31*value._43 - value._11*value._34*value._43 - value._13*value._31*value._44 - value._14*value._33*value._41;
    auto m23 = value._11*value._24*value._43 + value._13*value._21*value._44 + value._14*value._23*value._41 - value._11*value._23*value._44 - value._13*value._24*value._41 - value._14*value._21*value._43;
    auto m24 = value._11*value._23*value._34 + value._13*value._24*value._31 + value._14*value._21*value._33 - value._11*value._24*value._33 - value._13*value._21*value._34 - value._14*value._23*value._31;

    auto m31 = value._21*value._32*value._44 + value._22*value._34*value._41 + value._24*value._31*value._42 - value._21*value._34*value._42 - value._22*value._31*value._44 - value._24*value._32*value._41;
    auto m32 = value._11*value._34*value._42 + value._12*value._31*value._44 + value._14*value._32*value._41 - value._11*value._32*value._44 - value._12*value._34*value._41 - value._14*value._31*value._42;
    auto m33 = value._11*value._22*value._44 + value._12*value._24*value._41 + value._14*value._21*value._42 - value._11*value._24*value._42 - value._12*value._21*value._44 - value._14*value._22*value._41;
    auto m34 = value._11*value._24*value._32 + value._12*value._21*value._34 + value._14*value._22*value._31 - value._11*value._22*value._34 - value._12*value._24*value._31 - value._14*value._21*value._32;

    auto m41 = value._21*value._33*value._42 + value._22*value._31*value._43 + value._23*value._32*value._41 - value._21*value._32*value._43 - value._22*value._33*value._41 - value._23*value._31*value._42;
    auto m42 = value._11*value._32*value._43 + value._12*value._33*value._41 + value._13*value._31*value._42 - value._11*value._33*value._42 - value._12*value._31*value._43 - value._13*value._32*value._41;
    auto m43 = value._11*value._23*value._42 + value._12*value._21*value._43 + value._13*value._22*value._41 - value._11*value._22*value._43 - value._12*value._23*value._41 - value._13*value._21*value._42;
    auto m44 = value._11*value._22*value._33 + value._12*value._23*value._31 + value._13*value._21*value._32 - value._11*value._23*value._32 - value._12*value._21*value._33 - value._13*value._22*value._31;

    return Matrix(
        m11 / det, m12 / det, m13 / det, m14 / det,
        m21 / det, m22 / det, m23 / det, m24 / det,
        m31 / det, m32 / det, m33 / det, m34 / det,
        m41 / det, m42 / det, m43 / det, m44 / det );
}

inline Vector3 mul(const Vector3& lhs, const Matrix& rhs)
{
    return Vector3(
        (lhs.x * rhs._11) + (lhs.y * rhs._21) + (lhs.z * rhs._31) + rhs._41,
        (lhs.x * rhs._12) + (lhs.y * rhs._22) + (lhs.z * rhs._32) + rhs._42,
        (lhs.x * rhs._13) + (lhs.y * rhs._23) + (lhs.z * rhs._33) + rhs._43 );
}

inline Vector3 mul_normal(const Vector3& lhs, const Matrix& rhs)
{
    return Vector3(
        (lhs.x * rhs._11) + (lhs.y * rhs._21) + (lhs.z * rhs._31),
        (lhs.x * rhs._12) + (lhs.y * rhs._22) + (lhs.z * rhs._32),
        (lhs.x * rhs._13) + (lhs.y * rhs._23) + (lhs.z * rhs._33) );
}

inline Vector3 mul_coord(const Vector3& lhs, const Matrix& rhs)
{
    auto x = (lhs.x * rhs._11) + (lhs.y * rhs._21) + (lhs.z * rhs._31) + rhs._41;
    auto y = (lhs.x * rhs._12) + (lhs.y * rhs._22) + (lhs.z * rhs._32) + rhs._42;
    auto z = (lhs.x * rhs._13) + (lhs.y * rhs._23) + (lhs.z * rhs._33) + rhs._43;
    auto w = (lhs.x * rhs._14) + (lhs.y * rhs._24) + (lhs.z * rhs._34) + rhs._44;

    return Vector3(x/w, y/w, z/w);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Onb structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Onb
{
    static constexpr float Epsilon = 0.01f;

    Vector3 u;
    Vector3 v;
    Vector3 w;

    Onb& FromU(const Vector3& value)
    {
        auto n = Vector3(1, 0, 0);
        auto m = Vector3(0, 1, 0);

        u = normalize(value);
        v = cross(u, n);

        if (length(v) < Epsilon)
        { v = cross(u, m); }

        w = cross(u, v);

        return *this;
    }

    Onb& FromV(const Vector3& value)
    {
        auto n = Vector3(1, 0, 0);
        auto m = Vector3(0, 1, 0);

        v = normalize(value);
        u = cross(v, n);

        if (length(u) < Epsilon)
        { u = cross(v, m); }

        w = cross(u, v);

        return *this;
    }

    Onb& FromW(const Vector3& value)
    {
        auto n = Vector3(1, 0, 0);
        auto m = Vector3(0, 1, 0);

        w = normalize(value);
        u = cross(w, n);
        if (length(u) < Epsilon)
        { u = cross(w, m); }

        v = cross(w, u);

        return *this;
    }
};