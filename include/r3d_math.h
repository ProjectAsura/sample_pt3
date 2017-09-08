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

#if defined(ENABLE_SSE2)
#include <emmintrin.h>
#endif//defined(ENABLE_SSE2)

#if defined(ENABLE_AVX)
#include <immintrin.h>
#endif//defined(ENABLE_AVX)


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
    union 
    {
        struct 
        {
            float x;
            float y;
        };
        float a[2];
    };

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
    union
    {
        struct 
        {
            float x;
            float y;
            float z;
        };
        float a[3];
    };

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
};

inline Ray make_ray(const Vector3& pos, const Vector3& dir)
{
    Ray result;
    result.pos = pos;
    result.dir = dir;
    return result;
}

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

inline Vector2 abs(const Vector2& value)
{
    return Vector2(
        abs(value.x),
        abs(value.y)
    );
}

inline Vector3 abs(const Vector3& value)
{
    return Vector3(
        abs(value.x),
        abs(value.y),
        abs(value.z)
    );
}

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

inline Vector2 max(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(
        max(lhs.x, rhs.x),
        max(lhs.y, rhs.y)
    );
}

inline Vector3 max(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(
        max(lhs.x, rhs.x),
        max(lhs.y, rhs.y),
        max(lhs.z, rhs.z)
    );
}

inline Vector2 min(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2(
        min(lhs.x, rhs.x),
        min(lhs.y, rhs.y)
    );
}

inline Vector3 min(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3(
        min(lhs.x, rhs.x),
        min(lhs.y, rhs.y),
        min(lhs.z, rhs.z)
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Box structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Box
{
    Vector3 mini;
    Vector3 maxi;
    bool    empty;

    Box()
    : mini( F_MAX,  F_MAX,  F_MAX)
    , maxi(-F_MAX, -F_MAX, -F_MAX)
    , empty(true)
    { /* DO_NOTHING */ }

    Box(const Vector3& minimum, const Vector3& maximum)
    : mini (minimum)
    , maxi (maximum)
    , empty(false)
    { /* DO_NOTHING */ }
};

inline bool hit(const Ray& ray, const Box& box)
{
    float t0, t1, n, f;
    float t_min = -F_HIT_MAX;
    float t_max =  F_HIT_MAX;

    t0 = (box.mini.x - ray.pos.x) / ray.dir.x;
    t1 = (box.maxi.x - ray.pos.x) / ray.dir.x;

    n = min(t0, t1);
    f = max(t0, t1);

    t_min = max(t_min, n);
    t_max = min(t_max, f);

    if (t_min > t_max)
    { return false; }

    t0 = (box.mini.y - ray.pos.y) / ray.dir.y;
    t1 = (box.maxi.y - ray.pos.y) / ray.dir.y;

    n = min(t0, t1);
    f = max(t0, t1);

    t_min = max(t_min, n);
    t_max = min(t_max, f);

    if (t_min > t_max)
    { return false; }

    t0 = (box.mini.z - ray.pos.z) / ray.dir.z;
    t1 = (box.maxi.z - ray.pos.z) / ray.dir.z;

    n = min(t0, t1);
    f = max(t0, t1);

    t_min = max(t_min, n);
    t_max = min(t_max, f);

    if (t_min > t_max)
    { return false; }

    return true;
}

inline Box merge(const Box& lhs, const Box& rhs)
{
    if (lhs.empty)
    { return rhs; }
    else if (rhs.empty)
    { return lhs; }

    Box result;

    result.maxi.x = max(lhs.maxi.x, rhs.maxi.x);
    result.maxi.y = max(lhs.maxi.y, rhs.maxi.y);
    result.maxi.z = max(lhs.maxi.z, rhs.maxi.z);

    result.mini.x = min(lhs.mini.x, rhs.mini.x);
    result.mini.y = min(lhs.mini.y, rhs.mini.y);
    result.mini.z = min(lhs.mini.z, rhs.mini.z);

    return result;
}

inline Box merge(const Box& lhs, const Vector3& rhs)
{
    if (lhs.empty)
    { return Box(rhs, rhs); }

    Box result;
    result.maxi.x = max(lhs.maxi.x, rhs.x);
    result.maxi.y = max(lhs.maxi.y, rhs.y);
    result.maxi.z = max(lhs.maxi.z, rhs.z);

    result.mini.x = min(lhs.mini.x, rhs.x);
    result.mini.y = min(lhs.mini.y, rhs.y);
    result.mini.z = min(lhs.mini.z, rhs.z);

    return result;
}

inline Box merge(const Vector3& lhs, const Box& rhs)
{
    Box result;
    result.maxi.x = max(lhs.x, rhs.maxi.x);
    result.maxi.y = max(lhs.y, rhs.maxi.y);
    result.maxi.z = max(lhs.z, rhs.maxi.z);

    result.mini.x = min(lhs.x, rhs.mini.x);
    result.mini.y = min(lhs.y, rhs.mini.y);
    result.mini.z = min(lhs.z, rhs.mini.z);

    return result;
}

inline Box mul(const Box& box, const Matrix& matrix)
{
    auto mini = mul(box.mini, matrix);
    auto maxi = mul(box.maxi, matrix);
    return Box(mini, maxi);
}

inline float surface_area( const Vector3& a, const Vector3& b, const Vector3& c )
{
    auto X = ( b.y - a.y ) * ( c.z - a.z ) - ( c.y - a.y ) * ( b.z - a.z );
    auto Y = ( b.z - a.z ) * ( c.x - a.x ) - ( c.z - a.z ) * ( b.x - a.x );
    auto Z = ( b.x - a.x ) * ( c.y - a.y ) - ( c.x - a.x ) * ( b.y - a.y );
    return 0.5f * sqrtf( ( X * X ) + ( Y * Y ) + ( Z * Z ) );
}

inline float surface_area( const Box& box )
{
    auto X = fabs( box.maxi.x - box.mini.x );
    auto Y = fabs( box.maxi.y - box.mini.y );
    auto Z = fabs( box.maxi.z - box.mini.z );
    return 2.0f * ( ( X * Y ) + ( X * Z ) + ( Y * Z ) );
}

inline float surface_area( const float radius )
{
    return 4.0f * F_PI * ( radius * radius );
}


#if defined(ENABLE_SSE2)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Ray4 structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Ray4
{
    __m128  pos[3];
    __m128  dir[3];
};

inline Ray4 convert(const Ray& ray)
{
    Ray4 result;
    result.pos[0] = _mm_set1_ps( ray.pos.x );
    result.pos[1] = _mm_set1_ps( ray.pos.y );
    result.pos[2] = _mm_set1_ps( ray.pos.z );

    result.dir[0] = _mm_set1_ps( ray.dir.x );
    result.dir[1] = _mm_set1_ps( ray.dir.y );
    result.dir[2] = _mm_set1_ps( ray.dir.z );

    return result;
}

inline Ray revert(const Ray4& ray)
{
    Ray result;
    result.pos.x = _mm_cvtss_f32( ray.pos[0] );
    result.pos.y = _mm_cvtss_f32( ray.pos[1] );
    result.pos.z = _mm_cvtss_f32( ray.pos[2] );

    result.dir.x = _mm_cvtss_f32( ray.dir[0] );
    result.dir.y = _mm_cvtss_f32( ray.dir[1] );
    result.dir.z = _mm_cvtss_f32( ray.dir[2] );

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Box4 structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Box4
{
    __m128 mini[3];
    __m128 maxi[3];

    Box4()
    {
        mini[0] = _mm_set1_ps( F_MAX );
        mini[1] = _mm_set1_ps( F_MAX );
        mini[2] = _mm_set1_ps( F_MAX );

        maxi[0] = _mm_set1_ps( -F_MAX );
        maxi[1] = _mm_set1_ps( -F_MAX );
        maxi[2] = _mm_set1_ps( -F_MAX );
    }

    Box4(const Box& box)
    {
        mini[0] = _mm_set1_ps( box.mini.x );
        mini[1] = _mm_set1_ps( box.mini.y );
        mini[2] = _mm_set1_ps( box.mini.z );

        maxi[0] = _mm_set1_ps( box.maxi.x );
        maxi[1] = _mm_set1_ps( box.maxi.y );
        maxi[2] = _mm_set1_ps( box.maxi.z );
    }

    Box4(const Box& b0, const Box& b1, const Box& b2, const Box& b3)
    {
        mini[0] = _mm_set_ps( b3.mini.x, b2.mini.x, b1.mini.x, b0.mini.x );
        mini[1] = _mm_set_ps( b3.mini.y, b2.mini.y, b1.mini.y, b0.mini.y );
        mini[2] = _mm_set_ps( b3.mini.z, b2.mini.z, b1.mini.z, b0.mini.z );

        maxi[0] = _mm_set_ps( b3.maxi.x, b2.maxi.x, b1.maxi.x, b0.maxi.x );
        maxi[1] = _mm_set_ps( b3.maxi.y, b2.maxi.y, b1.maxi.y, b0.maxi.y );
        maxi[2] = _mm_set_ps( b3.maxi.z, b2.maxi.z, b1.maxi.z, b0.maxi.z );
    }
};

inline void revert(const Box4& box4, Box& b0, Box& b1, Box& b2, Box& b3)
{
    alignas(16) float mini_x[4];
    alignas(16) float mini_y[4];
    alignas(16) float mini_z[4];

    alignas(16) float maxi_x[4];
    alignas(16) float maxi_y[4];
    alignas(16) float maxi_z[4];

    _mm_store_ps(mini_x, box4.mini[0]);
    _mm_store_ps(mini_y, box4.mini[1]);
    _mm_store_ps(mini_z, box4.mini[2]);

    _mm_store_ps(maxi_x, box4.maxi[0]);
    _mm_store_ps(maxi_y, box4.maxi[1]);
    _mm_store_ps(maxi_z, box4.maxi[2]);

    b0.mini = Vector3(mini_x[0], mini_y[0], mini_z[0]);
    b0.maxi = Vector3(maxi_x[0], maxi_y[0], maxi_z[0]);

    b1.mini = Vector3(mini_x[1], mini_y[1], mini_z[1]);
    b1.maxi = Vector3(maxi_x[1], maxi_y[1], maxi_z[1]);

    b2.mini = Vector3(mini_x[2], mini_y[2], mini_z[2]);
    b2.maxi = Vector3(maxi_x[2], maxi_y[2], maxi_z[2]);

    b3.mini = Vector3(mini_x[3], mini_y[3], mini_z[3]);
    b3.maxi = Vector3(maxi_x[3], maxi_y[3], maxi_z[3]);
}

inline bool hit(const Ray4& ray, const Box4& box, int& mask)
{
    __m128 t_min = _mm_set1_ps( -F_HIT_MAX );
    __m128 t_max = _mm_set1_ps(  F_HIT_MAX );

    __m128 t0;
    __m128 t1;
    __m128 n;
    __m128 f;

    t0 = _mm_div_ps( _mm_sub_ps(box.mini[0], ray.pos[0]), ray.dir[0] );
    t1 = _mm_div_ps( _mm_sub_ps(box.maxi[0], ray.pos[0]), ray.dir[0] );

    n = _mm_min_ps( t0, t1 );
    f = _mm_max_ps( t0, t1 );

    t_min = _mm_max_ps( t_min, n );
    t_max = _mm_min_ps( t_max, f );

    t0 = _mm_div_ps( _mm_sub_ps(box.mini[1], ray.pos[1]), ray.dir[1] );
    t1 = _mm_div_ps( _mm_sub_ps(box.maxi[1], ray.pos[1]), ray.dir[1] );

    n = _mm_min_ps( t0, t1 );
    f = _mm_max_ps( t0, t1 );

    t_min = _mm_max_ps( t_min, n );
    t_max = _mm_min_ps( t_max, f );

    t0 = _mm_div_ps( _mm_sub_ps(box.mini[2], ray.pos[2]), ray.dir[2] );
    t1 = _mm_div_ps( _mm_sub_ps(box.maxi[2], ray.pos[2]), ray.dir[2] );

    n = _mm_min_ps( t0, t1 );
    f = _mm_max_ps( t0, t1 );

    t_min = _mm_max_ps( t_min, n );
    t_max = _mm_min_ps( t_max, f );

    mask = _mm_movemask_ps( _mm_cmple_ps( t_min, t_max ) );
    return mask > 0;
}

inline bool hit_non_simd(const Ray4& ray, const Box4& box, int& mask)
{
    Ray r = revert(ray);

    Box b0, b1, b2, b3;
    revert(box, b0, b1, b2, b3);

    auto hit0 = hit(r, b0) ? 1 : 0;
    auto hit1 = hit(r, b1) ? 1 : 0;
    auto hit2 = hit(r, b2) ? 1 : 0;
    auto hit3 = hit(r, b3) ? 1 : 0;

    mask = (hit3 << 3) | (hit2 << 2) | (hit1 << 1) | (hit0 << 0);
    return mask > 0;
}
#endif//defined(ENABLE_SSE2)


#if defined(ENABLE_AVX)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Ray8 structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Ray8
{
    __m256 pos[3];
    __m256 dir[3];
};

inline Ray8 convert(const Ray& ray)
{
    Ray8 result;
    result.pos[0] = _mm256_set1_ps( ray.pos.x );
    result.pos[1] = _mm256_set1_ps( ray.pos.y );
    result.pos[2] = _mm256_set1_ps( ray.pos.z );

    result.dir[0] = _mm256_set1_ps( ray.dir.x );
    result.dir[1] = _mm256_set1_ps( ray.dir.y );
    result.dir[2] = _mm256_set1_ps( ray.dir.z );

    return result;
}

inline Ray revert(const Ray8& ray)
{
    Ray result;
    result.pos.x = _mm256_cvtss_f32( ray.pos[0] );
    result.pos.y = _mm256_cvtss_f32( ray.pos[1] );
    result.pos.z = _mm256_cvtss_f32( ray.pos[2] );

    result.dir.x = _mm256_cvtss_f32( ray.dir[0] );
    result.dir.y = _mm256_cvtss_f32( ray.dir[1] );
    result.dir.z = _mm256_cvtss_f32( ray.dir[2] );

    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Box8 structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Box8
{
    __m256 mini[3];
    __m256 maxi[3];

    Box8()
    {
        mini[0] = _mm256_set1_ps( F_MAX );
        mini[1] = _mm256_set1_ps( F_MAX );
        mini[2] = _mm256_set1_ps( F_MAX );

        maxi[0] = _mm256_set1_ps( -F_MAX );
        maxi[1] = _mm256_set1_ps( -F_MAX );
        maxi[2] = _mm256_set1_ps( -F_MAX );
    }

    Box8(const Box& box)
    {
        mini[0] = _mm256_set1_ps( box.mini.x );
        mini[1] = _mm256_set1_ps( box.mini.y );
        mini[2] = _mm256_set1_ps( box.mini.z );

        maxi[0] = _mm256_set1_ps( box.maxi.x );
        maxi[1] = _mm256_set1_ps( box.maxi.y );
        maxi[2] = _mm256_set1_ps( box.maxi.z );
    }

    Box8(const Box& b0, const Box& b1, const Box& b2, const Box& b3,
         const Box& b4, const Box& b5, const Box& b6, const Box& b7)
    {
        mini[0] = _mm256_set_ps( b7.mini.x, b6.mini.x, b5.mini.x, b4.mini.x, b3.mini.x, b2.mini.x, b1.mini.x, b0.mini.x );
        mini[1] = _mm256_set_ps( b7.mini.y, b6.mini.y, b5.mini.y, b4.mini.y, b3.mini.y, b2.mini.y, b1.mini.y, b0.mini.y );
        mini[2] = _mm256_set_ps( b7.mini.z, b6.mini.z, b5.mini.z, b4.mini.z, b3.mini.z, b2.mini.z, b1.mini.z, b0.mini.z );

        maxi[0] = _mm256_set_ps( b7.maxi.x, b6.maxi.x, b5.maxi.x, b4.maxi.x, b3.maxi.x, b2.maxi.x, b1.maxi.x, b0.maxi.x );
        maxi[1] = _mm256_set_ps( b7.maxi.y, b6.maxi.y, b5.maxi.y, b4.maxi.y, b3.maxi.y, b2.maxi.y, b1.maxi.y, b0.maxi.y );
        maxi[2] = _mm256_set_ps( b7.maxi.z, b6.maxi.z, b5.maxi.z, b4.maxi.z, b3.maxi.z, b2.maxi.z, b1.maxi.z, b0.maxi.z );
    }
};

inline void revert(const Box8& box8, Box& b0, Box& b1, Box& b2, Box& b3, Box& b4, Box& b5, Box& b6, Box& b7)
{
    alignas(32) float mini_x[8];
    alignas(32) float mini_y[8];
    alignas(32) float mini_z[8];

    alignas(32) float maxi_x[8];
    alignas(32) float maxi_y[8];
    alignas(32) float maxi_z[8];

    _mm256_store_ps(mini_x, box8.mini[0]);
    _mm256_store_ps(mini_y, box8.mini[1]);
    _mm256_store_ps(mini_z, box8.mini[2]);

    _mm256_store_ps(maxi_x, box8.maxi[0]);
    _mm256_store_ps(maxi_y, box8.maxi[1]);
    _mm256_store_ps(maxi_z, box8.maxi[2]);

    b0.mini = Vector3(mini_x[0], mini_y[0], mini_z[0]);
    b0.maxi = Vector3(maxi_x[0], maxi_y[0], maxi_z[0]);

    b1.mini = Vector3(mini_x[1], mini_y[1], mini_z[1]);
    b1.maxi = Vector3(maxi_x[1], maxi_y[1], maxi_z[1]);

    b2.mini = Vector3(mini_x[2], mini_y[2], mini_z[2]);
    b2.maxi = Vector3(maxi_x[2], maxi_y[2], maxi_z[2]);

    b3.mini = Vector3(mini_x[3], mini_y[3], mini_z[3]);
    b3.maxi = Vector3(maxi_x[3], maxi_y[3], maxi_z[3]);

    b4.mini = Vector3(mini_x[4], mini_y[4], mini_z[4]);
    b4.maxi = Vector3(maxi_x[4], maxi_y[4], maxi_z[4]);

    b5.mini = Vector3(mini_x[5], mini_y[5], mini_z[5]);
    b5.maxi = Vector3(maxi_x[5], maxi_y[5], maxi_z[5]);

    b6.mini = Vector3(mini_x[6], mini_y[6], mini_z[6]);
    b6.maxi = Vector3(maxi_x[6], maxi_y[6], maxi_z[6]);

    b7.mini = Vector3(mini_x[7], mini_y[7], mini_z[7]);
    b7.maxi = Vector3(maxi_x[7], maxi_y[7], maxi_z[7]);
}

inline bool hit(const Ray8& ray, const Box8& box, int& mask)
{
    __m256 t_min = _mm256_set1_ps( -F_HIT_MAX );
    __m256 t_max = _mm256_set1_ps(  F_HIT_MAX );

    __m256 t0;
    __m256 t1;
    __m256 n;
    __m256 f;

    t0 = _mm256_div_ps( _mm256_sub_ps(box.mini[0], ray.pos[0]), ray.dir[0] );
    t1 = _mm256_div_ps( _mm256_sub_ps(box.maxi[0], ray.pos[0]), ray.dir[0] );

    n = _mm256_min_ps( t0, t1 );
    f = _mm256_max_ps( t0, t1 );

    t_min = _mm256_max_ps( t_min, n );
    t_max = _mm256_min_ps( t_max, f );

    t0 = _mm256_div_ps( _mm256_sub_ps(box.mini[1], ray.pos[1]), ray.dir[1] );
    t1 = _mm256_div_ps( _mm256_sub_ps(box.maxi[1], ray.pos[1]), ray.dir[1] );

    n = _mm256_min_ps( t0, t1 );
    f = _mm256_max_ps( t0, t1 );

    t_min = _mm256_max_ps( t_min, n );
    t_max = _mm256_min_ps( t_max, f );

    t0 = _mm256_div_ps( _mm256_sub_ps(box.mini[2], ray.pos[2]), ray.dir[2] );
    t1 = _mm256_div_ps( _mm256_sub_ps(box.maxi[2], ray.pos[2]), ray.dir[2] );

    n = _mm256_min_ps( t0, t1 );
    f = _mm256_max_ps( t0, t1 );

    t_min = _mm256_max_ps( t_min, n );
    t_max = _mm256_min_ps( t_max, f );

    mask = _mm256_movemask_ps( _mm256_cmp_ps( t_max, t_min, _CMP_GE_OS ) );

    return mask > 0;
}

inline bool hit_non_simd(const Ray8& ray, const Box8& box, int& mask)
{
    Ray r = revert(ray);

    Box b0, b1, b2, b3, b4, b5, b6, b7
    revert(box, b0, b1, b2, b3, b6, b7);

    auto hit0 = hit(r, b0) ? 1 : 0;
    auto hit1 = hit(r, b1) ? 1 : 0;
    auto hit2 = hit(r, b2) ? 1 : 0;
    auto hit3 = hit(r, b3) ? 1 : 0;
    auto hit4 = hit(r, b4) ? 1 : 0;
    auto hit5 = hit(r, b5) ? 1 : 0;
    auto hit6 = hit(r, b6) ? 1 : 0;
    auto hit7 = hit(r, b7) ? 1 : 0;

    mask  = (hit0 << 0);
    mask |= (hit1 << 1);
    mask |= (hit2 << 2);
    mask |= (hit3 << 3);
    mask |= (hit4 << 4);
    mask |= (hit5 << 5);
    mask |= (hit6 << 6);
    mask |= (hit7 << 7);

    return mask > 0;
}
#endif//defined(ENABLE_AVX)
