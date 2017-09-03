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
#include <new>


///////////////////////////////////////////////////////////////////////////////////////////////////
// ShadingArg structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct ShadingArg
{
    Vector3     input;          // 入射方向         [in].
    Vector3     output;         // 出射方向         [out].
    Vector3     normal;         // 法線ベクトル      [in].
    Vector2     uv;             // テクスチャ座標    [in].
    Random      random;         // 乱数             [in, out].
    float       pdf;            // BRDFの確率密度   [out].
};


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

    static constexpr float kThresholdEps = 1e-3f;

    virtual Type    type() const = 0;
    virtual bool    is_delta() const = 0;
    virtual float   threshold() const = 0;
    virtual Vector3 shade(ShadingArg& arg) const = 0;
    virtual Vector3 emissive() const = 0;
};

class Lambert : public Material
{
public:
    static Material* create(const Vector3& albedo)
    { return create(albedo, Vector3(0.0f, 0.0f, 0.0f)); }

    static Material* create(const Vector3& albedo, const Vector3& emissive)
    {
        auto instance = new (std::nothrow) Lambert();
        instance->m_albedo    = albedo;
        instance->m_emissive  = emissive;
        instance->m_threshold = max(albedo.x, max(albedo.y, albedo.z));
        instance->m_threshold = max(instance->m_threshold, kThresholdEps);
        return instance;
    }

    Type type() const override
    { return Type::Lambert; }

    bool is_delta() const override
    { return false; }

    float threshold() const override
    { return m_threshold; }

    Vector3 emissive() const override
    { return m_emissive; }

    Vector3 shade(ShadingArg& arg) const override
    {
        // 物体からのレイの入出を考慮した法線ベクトル.
        auto normal = (dot(arg.normal, arg.input) < 0.0f) ? arg.normal : -arg.normal;

        // 基底ベクトル.
        Onb onb;
        onb.FromW(normal);

        const auto r1 = F_2PI * arg.random.get_as_float();
        const auto r2 = arg.random.get_as_float();
        const auto r2s = sqrt(r2);

        // 出射方向.
        arg.output = normalize(onb.u * cos(r1) * r2s + onb.v * sin(r1) * r2s + onb.w * sqrt(1.0 - r2));

        const auto cosine = dot(normal, arg.output);

        // 確率密度.
        arg.pdf = cosine / F_PI;

        return m_albedo;
    }

private:
    Vector3 m_albedo;
    Vector3 m_emissive;
    float   m_threshold;
};

class Mirror : public Material
{
public:
    static Mirror* create(const Vector3& albedo)
    { return create(albedo, Vector3(0.0f, 0.0f, 0.0f)); }

    static Mirror* create(const Vector3& albedo, const Vector3& emissive)
    {
        auto instance = new (std::nothrow) Mirror();
        instance->m_albedo   = albedo;
        instance->m_emissive = emissive;
        instance->m_threshold = max(albedo.x, max(albedo.y, albedo.z));
        instance->m_threshold = max(instance->m_threshold, kThresholdEps);
        return instance;
    }

    Type type() const override
    { return Type::Mirror; }

    bool is_delta() const override
    { return true; }

    float threshold() const override
    { return m_threshold; }

    Vector3 emissive() const override
    { return m_emissive; }

    Vector3 shade(ShadingArg& arg) const override
    {
        // 物体からのレイの入出を考慮した法線ベクトル.
        auto normal = (dot(arg.normal, arg.input) < 0.0f) ? arg.normal : -arg.normal;

        arg.output = reflect(arg.input, normal);
        arg.pdf    = 1.0f;
        return m_albedo;
    }

private:
    Vector3 m_albedo;
    Vector3 m_emissive;
    float   m_threshold;
};

class Refract : public Material
{
public:
    static Refract* create(const Vector3& albedo, float ior)
    { return create(albedo, ior, Vector3(0.0f, 0.0f, 0.0f)); }

    static Refract* create(const Vector3& albedo, float ior, const Vector3& emissive)
    {
        auto instance = new (std::nothrow) Refract();
        instance->m_albedo    = albedo;
        instance->m_emissive  = emissive;
        instance->m_ior       = ior;
        instance->m_threshold = max(albedo.x, max(albedo.y, albedo.z));
        instance->m_threshold = max(instance->m_threshold, kThresholdEps);
        return instance;
    }

    Type type() const override
    { return Type::Refract; }

    bool is_delta() const override
    { return true; }

    float threshold() const override
    { return m_threshold; }

    Vector3 emissive() const override
    { return m_emissive; }

    Vector3 shade(ShadingArg& arg) const override
    {
        arg.pdf = 1.0f;

        // 物体からのレイの入出を考慮した法線ベクトル.
        auto normal = (dot(arg.normal, arg.input) < 0.0f) ? arg.normal : -arg.normal;
        auto into = dot(arg.normal, normal) > 0.0f;

        const auto nc = 1.0f;
        const auto nt = m_ior;
        const auto nnt = (into) ? (nc / nt) : (nt / nc);
        const auto ddn = dot(arg.input, normal);
        const auto cos2t = 1.0f - nnt * nnt * (1.0f - ddn * ddn);

        if (cos2t <= 0.0f)
        {
            arg.output = reflect(arg.input, arg.normal);
            return m_albedo;
        }

        auto dir = normalize(arg.input * nnt - arg.normal * ((into) ? 1.0f : -1.0f) * (ddn * nnt + sqrt(cos2t)));

        const auto a = nt - nc;
        const auto b = nt + nc;
        const auto R0 = (a * a) / (b * b);
        const auto c = 1.0f - ((into) ? -ddn : dot(dir, arg.normal));
        const auto Re = R0 + (1.0f - R0) * pow(c, 5.0f);
        const auto Tr = 1.0f - Re;
        const auto prob = 0.25f + 0.5f * Re;

        if (arg.random.get_as_float() < prob)
        {
            arg.output = reflect(arg.input, arg.normal);
            return m_albedo * Re / prob;
        }
        else
        {
            arg.output = dir;
            return m_albedo * Tr / (1.0f - prob);
        }
    }

private:
    Vector3 m_albedo;
    Vector3 m_emissive;
    float   m_ior;
    float   m_threshold;
};


class Phong : public Material
{
public:
    static Phong* create(const Vector3& albedo, float shininess)
    { return create(albedo, shininess, Vector3(0.0f, 0.0f, 0.0f)); }

    static Phong* create(const Vector3& albedo, float shininess, const Vector3& emissive)
    {
        auto instance = new (std::nothrow) Phong();
        instance->m_albedo    = albedo;
        instance->m_emissive  = emissive;
        instance->m_shininess = shininess;
        instance->m_threshold = max(albedo.x, max(albedo.y, albedo.z));
        instance->m_threshold = max(instance->m_threshold, kThresholdEps);
        return instance;
    }

    Type type() const override
    { return Type::Phong; }

    bool is_delta() const override
    { return false; }

    float threshold() const override
    { return m_threshold; }

    Vector3 emissive() const override
    { return m_emissive; }

    Vector3 shade(ShadingArg& arg) const override
    {
        // 物体からのレイの入出を考慮した法線ベクトル.
        auto normal = (dot(arg.normal, arg.input) < 0.0f) ? arg.normal : -arg.normal;

        const auto phi = F_2PI * arg.random.get_as_float();
        const auto cos_theta = pow( 1.0f - arg.random.get_as_float(), 1.0f / (m_shininess + 1.0f) );
        const auto sin_theta = sqrt( 1.0f - (cos_theta * cos_theta) );
        const auto x = cos( phi ) * sin_theta;
        const auto y = sin( phi ) * sin_theta;
        const auto z = cos_theta;

        auto w = reflect(arg.input, normal);
        Onb onb;
        onb.FromW(w);

        auto dir = normalize(onb.u * x + onb.v * y + onb.w * z);
        auto cosine = dot(dir, normal);

        arg.output = dir;
        arg.pdf    = ((m_shininess + 1.0f) / F_2PI) * cosine;

        return m_albedo * cosine * ((m_shininess + 2.0) / (m_shininess + 1.0f));
    }

private:
    Vector3 m_albedo;
    Vector3 m_emissive;
    float   m_shininess;
    float   m_threshold;
};
