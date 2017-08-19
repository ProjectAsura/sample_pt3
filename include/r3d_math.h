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

///////////////////////////////////////////////////////////////////////////////////////////////////
// Vector2 structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vector2
{
    float x = 0.0f;
    float y = 0.0f;

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
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

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
    Random(uint32_t seed)
    { set_seed( 123456789 ); }

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