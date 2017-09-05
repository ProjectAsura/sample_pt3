//-------------------------------------------------------------------------------------------------
// File : r3d_scene.cpp
// Desc : Scene.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_scene.h>
#include <string>
#include <cassert>
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>


template<typename Archive>
void serialize(Archive& archive, Vector2& value)
{
    archive(
        cereal::make_nvp("x", value.x),
        cereal::make_nvp("y", value.y)
    );
}

template<typename Archive>
void serialize(Archive& archive, Vector3& value)
{
    archive(
        cereal::make_nvp("x", value.x),
        cereal::make_nvp("y", value.y),
        cereal::make_nvp("z", value.z)
    );
}

template<typename Archive>
void serialize(Archive& archive, Matrix& value)
{
    archive(
        cereal::make_nvp("_11", value._11),
        cereal::make_nvp("_12", value._12),
        cereal::make_nvp("_13", value._13),
        cereal::make_nvp("_14", value._14),

        cereal::make_nvp("_21", value._21),
        cereal::make_nvp("_22", value._22),
        cereal::make_nvp("_23", value._23),
        cereal::make_nvp("_24", value._24),

        cereal::make_nvp("_31", value._31),
        cereal::make_nvp("_32", value._32),
        cereal::make_nvp("_33", value._33),
        cereal::make_nvp("_34", value._34),

        cereal::make_nvp("_41", value._41),
        cereal::make_nvp("_42", value._42),
        cereal::make_nvp("_43", value._43),
        cereal::make_nvp("_44", value._44)
    );
}

struct ResTexture
{
    int                 id;
    std::string         path;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(id),
            CEREAL_NVP(path)
        );
    }
};

struct ResLambert
{
    int         id;
    Vector3     color;
    Vector3     emissive;
    int         texture_id;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(id),
            CEREAL_NVP(color),
            CEREAL_NVP(emissive),
            CEREAL_NVP(texture_id)
        );
    }
};

struct ResMirror
{
    int         id;
    Vector3     color;
    Vector3     emissive;
    int         texture_id;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(id),
            CEREAL_NVP(color),
            CEREAL_NVP(emissive),
            CEREAL_NVP(texture_id)
        );
    }
};

struct ResRefract
{
    int         id;
    Vector3     color;
    Vector3     emissive;
    float       ior;
    int         texture_id;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(id),
            CEREAL_NVP(color),
            CEREAL_NVP(emissive),
            CEREAL_NVP(ior),
            CEREAL_NVP(texture_id)
        );
    }
};


struct ResPhong
{
    int         id;
    Vector3     color;
    Vector3     emissive;
    float       shininess;
    int         texture_id;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(color),
            CEREAL_NVP(emissive),
            CEREAL_NVP(shininess),
            CEREAL_NVP(texture_id)
        );
    }
};

struct ResSphere
{
    int         id;
    Vector3     pos;
    float       radius;
    int         material_id;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(id),
            CEREAL_NVP(pos),
            CEREAL_NVP(radius),
            CEREAL_NVP(material_id)
        );
    }
};

struct ResShapeInstance
{
    int     id;
    Matrix  world;
    int     shape_id;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(id),
            CEREAL_NVP(world),
            CEREAL_NVP(shape_id)
        );
    }
};

struct ResCamera
{
    Vector3     pos;
    Vector3     dir;
    Vector3     upward;
    float       fov;
    float       aspect;
    float       znear;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(pos),
            CEREAL_NVP(dir),
            CEREAL_NVP(upward),
            CEREAL_NVP(fov),
            CEREAL_NVP(aspect),
            CEREAL_NVP(znear)
        );
    }
};

struct ResThinLensCamera
{
    Vector3     pos;
    Vector3     dir;
    Vector3     upward;
    float       fov;
    float       aspect;
    float       znear;
    float       radius;
    float       focal_dist;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(pos),
            CEREAL_NVP(dir),
            CEREAL_NVP(upward),
            CEREAL_NVP(fov),
            CEREAL_NVP(aspect),
            CEREAL_NVP(znear),
            CEREAL_NVP(radius),
            CEREAL_NVP(focal_dist)
        );
    }
};

struct ResScene
{
    int                             width;
    int                             height;
    int                             samples;
    std::vector<ResTexture>         textures;
    std::vector<ResLambert>         lambers;
    std::vector<ResMirror>          mirrors;
    std::vector<ResRefract>         refracts;
    std::vector<ResPhong>           phongs;
    std::vector<ResSphere>          sphere_shapes;
    std::vector<ResShapeInstance>   instance_shapes;
    std::vector<ResCamera>          cameras;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(width),
            CEREAL_NVP(height),
            CEREAL_NVP(samples),
            CEREAL_NVP(textures),
            CEREAL_NVP(lambers),
            CEREAL_NVP(mirrors),
            CEREAL_NVP(refracts),
            CEREAL_NVP(phongs),
            CEREAL_NVP(sphere_shapes),
            CEREAL_NVP(cameras)
        );
    }

    void default_scene()
    {
    }
};


Scene::Scene()
: m_cam(nullptr)
{ /* DO_NOTHING */ }

Scene::~Scene()
{ dispose(); }

bool Scene::load(const char* filename)
{
    dispose();

    std::ifstream stream(filename);

    if (!stream.is_open())
    { return false; }

    {
        ResScene res;
        cereal::JSONInputArchive arc(stream);
        arc(cereal::make_nvp("scene", res));

        // TODO : convert.
    }

    return true;
}

bool Scene::save(const char* filename)
{
    std::ofstream stream(filename);

    if (!stream.is_open())
    { return false; }

    {
        ResScene res;

        // TODO : convert.

        cereal::JSONOutputArchive arc(stream);
        arc(cereal::make_nvp("scene", res));
    }

    return true;
}

void Scene::dispose()
{
    for(size_t i=0; i<m_texs.size(); ++i)
    {
        if (m_texs[i] != nullptr)
        {
            delete m_texs[i];
            m_texs[i] = nullptr;
        }
    }

    for(size_t i=0; i<m_objs.size(); ++i)
    {
        if (m_objs[i] != nullptr)
        {
            delete m_objs[i];
            m_objs[i] = nullptr;
        }
    }

    for(size_t i=0; i<m_mats.size(); ++i)
    {
        if (m_mats[i] != nullptr)
        {
            delete m_mats[i];
            m_mats[i] = nullptr;
        }
    }

    if (m_cam != nullptr)
    {
        delete m_cam;
        m_cam = nullptr;
    }

    m_texs.clear();
    m_objs.clear();
    m_mats.clear();

    m_w = 0;
    m_h = 0;
    m_s = 0;
}

Ray Scene::emit(float x, float y) const
{ return m_cam->emit(x, y); }

bool Scene::hit(const Ray& ray, HitRecord& record) const
{
    record.dist  = F_MAX;
    record.shape = nullptr;
    record.mat   = nullptr;

    bool hit = false;

    for(size_t i=0; i<m_objs.size(); ++i)
    { hit |= m_objs[i]->hit(ray, record); }

    return hit;
}

bool Scene::hit(const Ray& ray, ShadowRecord& record) const
{
    record.dist  = F_MAX;
    record.shape = nullptr;
    record.mat   = nullptr;

    bool hit = false;

    for(size_t i=0; i<m_objs.size(); ++i)
    { hit |= m_objs[i]->shadow_hit(ray, record); }

    return hit;
}