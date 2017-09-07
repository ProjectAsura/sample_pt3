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
#include <map>
#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
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
    float       znear;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(pos),
            CEREAL_NVP(dir),
            CEREAL_NVP(upward),
            CEREAL_NVP(fov),
            CEREAL_NVP(znear)
        );
    }
};

struct ResThinLensCamera
{
    Vector3     pos;
    Vector3     dir;
    Vector3     upward;
    float       fov_deg;
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
            CEREAL_NVP(fov_deg),
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
    std::vector<ResLambert>         lamberts;
    std::vector<ResMirror>          mirrors;
    std::vector<ResRefract>         refracts;
    std::vector<ResPhong>           phongs;
    std::vector<ResSphere>          sphere_shapes;
    std::vector<ResShapeInstance>   instance_shapes;
    std::vector<ResCamera>          cameras;
    std::string                     ibl_path;

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            CEREAL_NVP(width),
            CEREAL_NVP(height),
            CEREAL_NVP(samples),
            CEREAL_NVP(textures),
            CEREAL_NVP(lamberts),
            CEREAL_NVP(mirrors),
            CEREAL_NVP(refracts),
            CEREAL_NVP(phongs),
            CEREAL_NVP(sphere_shapes),
            CEREAL_NVP(cameras),
            CEREAL_NVP(ibl_path)
        );
    }

    void default_scene()
    {
        int id = 1;

        //width = 1280;
        //height = 960;
        width = 480;
        height = 270;
        samples = 512;

        ResLambert lambert0 = {};
        lambert0.id = id++;
        lambert0.color = Vector3(0.25f, 0.75f, 0.25f);
        lambert0.emissive = Vector3(0.0f, 0.0f, 0.0f);

        ResLambert lambert1 = {};
        lambert1.id = id++;
        lambert1.color = Vector3(0.25f, 0.25f, 0.75f);
        lambert1.emissive = Vector3(0.0f, 0.0f, 0.0f);

        ResLambert lambert2 = {};
        lambert2.id = id++;
        lambert2.color = Vector3(0.75f, 0.75f, 0.75f);
        lambert2.emissive = Vector3(0.0f, 0.0f, 0.0f);

        ResLambert lambert3 = {};
        lambert3.id = id++;
        lambert3.color = Vector3(0.01f, 0.01f, 0.01f);
        lambert3.emissive = Vector3(0.0f, 0.0f, 0.0f);

        ResLambert lambert4 = {};
        lambert4.id = id++;
        lambert4.color = Vector3(0.0f, 0.0f, 0.0f);
        lambert4.emissive = Vector3(12.0f, 12.0f, 12.0f);

        lamberts.push_back(lambert0);
        lamberts.push_back(lambert1);
        lamberts.push_back(lambert2);
        lamberts.push_back(lambert3);
        lamberts.push_back(lambert4);

        ResMirror mirror0 = {};
        mirror0.id      = id++;
        mirror0.color   = Vector3(0.75f, 0.25f, 0.25f);
        mirror0.emissive = Vector3(0.0f, 0.0f, 0.0f);

        mirrors.push_back(mirror0);

        ResRefract refract0 = {};
        refract0.id    = id++;
        refract0.color = Vector3(0.99f, 0.99f, 0.99f);
        refract0.ior   = 1.5f;
        refract0.emissive = Vector3(0.0f, 0.0f, 0.0f);

        refracts.push_back(refract0);

        id = 1;
        ResSphere sphere0 = {};
        sphere0.id          = id++;
        sphere0.radius      = 1e5f;
        sphere0.pos         = Vector3(1e5f + 1.0f, 40.8f, 81.6f);
        sphere0.material_id = lambert0.id;

        ResSphere sphere1 = {};
        sphere1.id          = id++;
        sphere1.radius      = 1e5f;
        sphere1.pos         = Vector3(-1e5f + 99.0f, 40.8f, 81.6f);
        sphere1.material_id = lambert1.id;

        ResSphere sphere2 = {};
        sphere2.id          = id++;
        sphere2.radius      = 1e5f;
        sphere2.pos         = Vector3(50.0f, 40.8f, 1e5f);
        sphere2.material_id = lambert2.id;

        ResSphere sphere3 = {};
        sphere3.id          = id++;
        sphere3.radius      = 1e5f;
        sphere3.pos         = Vector3(50.0f, 40.8f, -1e5f + 170.0f);
        sphere3.material_id = lambert3.id;

        ResSphere sphere4 = {};
        sphere4.id          = id++;
        sphere4.radius      = 1e5f;
        sphere4.pos         = Vector3(50.0f, 1e5f, 81.6f);
        sphere4.material_id = lambert2.id;

        ResSphere sphere5= {};
        sphere5.id          = id++;
        sphere5.radius      = 1e5f;
        sphere5.pos         = Vector3(50.0f, -1e5f + 81.6f, 81.6f);
        sphere5.material_id = lambert2.id;

        ResSphere sphere6 = {};
        sphere6.id          = id++;
        sphere6.radius      = 16.5f;
        sphere6.pos         = Vector3(27.0f, 16.5f, 47.0f);
        sphere6.material_id = mirror0.id;

        ResSphere sphere7 = {};
        sphere7.id          = id++;
        sphere7.radius      = 16.5f;
        sphere7.pos         = Vector3(73.0f, 16.5f, 78.0f);
        sphere7.material_id = refract0.id;

        ResSphere sphere8 = {};
        sphere8.id          = id++;
        sphere8.radius      = 5.0f;
        sphere8.pos         = Vector3(50.0f, 81.6f, 81.6f);
        sphere8.material_id = lambert4.id;

        sphere_shapes.push_back(sphere0);
        sphere_shapes.push_back(sphere1);
        sphere_shapes.push_back(sphere2);
        sphere_shapes.push_back(sphere3);
        sphere_shapes.push_back(sphere4);
        sphere_shapes.push_back(sphere5);
        sphere_shapes.push_back(sphere6);
        sphere_shapes.push_back(sphere7);
        sphere_shapes.push_back(sphere8);

        ResCamera camera = {};
        camera.pos      = Vector3(50.0f, 52.0f, 295.6f);
        camera.dir      = normalize(Vector3(0.0f, -0.042612f, -1.0f));
        camera.upward   = Vector3(0.0f, 1.0f, 0.0f);
        camera.fov      = 45.0f;
        camera.znear    = 130.0f;

        cameras.push_back(camera);

        ibl_path = "HDR_041_Path.hdr";
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
        cereal::XMLInputArchive arc(stream);
        arc(cereal::make_nvp("scene", res));

        m_w = res.width;
        m_h = res.height;
        m_s = res.samples;

        if (!res.textures.empty())
        {
            m_texs.resize(res.textures.size());
            for(size_t i=0; i<m_texs.size(); ++i)
            {
                m_texs[i] = new(std::nothrow) Texture();
                if (!m_texs[i]->load(res.textures[i].path.c_str()))
                {
                    fprintf_s(stderr, "Error : Texture Load Failed. %s\n", res.textures[i].path.c_str());
                }
            }
        }

        std::map<int, size_t> matid_dic;

        if (!res.lamberts.empty())
        {
            for(size_t i=0; i<res.lamberts.size(); ++i)
            {
                auto lambert = Lambert::create(res.lamberts[i].color, res.lamberts[i].emissive);
                auto idx = m_mats.size();
                m_mats.push_back(lambert);
                matid_dic[res.lamberts[i].id] = idx;
            }
        }

        if (!res.mirrors.empty())
        {
            for(size_t i=0; i<res.mirrors.size(); ++i)
            {
                auto mirror = Mirror::create(res.mirrors[i].color, res.mirrors[i].emissive);
                auto idx = m_mats.size();
                m_mats.push_back(mirror);
                matid_dic[res.mirrors[i].id] = idx;
            }
        }

        if (!res.refracts.empty())
        {
            for(size_t i=0; i<res.refracts.size(); ++i)
            {
                auto refract = Refract::create(res.refracts[i].color, res.refracts[i].ior, res.refracts[i].emissive);
                auto idx = m_mats.size();
                m_mats.push_back(refract);
                matid_dic[res.refracts[i].id] = idx;
            }
        }

        if (!res.phongs.empty())
        {
            for(size_t i=0; i<res.phongs.size(); ++i)
            {
                auto phong = Phong::create(res.phongs[i].color, res.phongs[i].shininess, res.phongs[i].emissive);
                auto idx = m_mats.size();
                matid_dic[res.phongs[i].id] = idx;
            }
        }

        m_mats.shrink_to_fit();

        std::map<int, size_t> shapeid_dic;

        if (!res.sphere_shapes.empty())
        {
            for(size_t i=0; i<res.sphere_shapes.size(); ++i)
            {
                auto idx = matid_dic[res.sphere_shapes[i].material_id];
                auto mat = m_mats[idx];
                auto shape = Sphere::create(res.sphere_shapes[i].radius, res.sphere_shapes[i].pos, mat);
                auto id = m_objs.size();
                m_objs.push_back(shape);
                shapeid_dic[res.sphere_shapes[i].id] = id;
            }
        }

        if (!res.instance_shapes.empty())
        {
            for(size_t i=0; i<res.instance_shapes.size(); ++i)
            {
                auto idx = shapeid_dic[res.instance_shapes[i].shape_id];
                auto obj = m_objs[idx];
                auto shape = ShapeInstance::create(obj, res.instance_shapes[i].world);
                auto id = m_objs.size();
                m_objs.push_back(shape);
                shapeid_dic[res.instance_shapes[i].id] = id;
            }
        }

        m_objs.shrink_to_fit();

        if (!res.cameras.empty())
        {
            m_cam = new (std::nothrow) Camera(
                res.cameras[0].pos,
                res.cameras[0].dir,
                res.cameras[0].upward,
                radian(res.cameras[0].fov),
                res.cameras[0].znear,
                float(m_w),
                float(m_h));
        }

        if (!res.ibl_path.empty())
        {
            m_ibl = new (std::nothrow) Texture();
            if (!m_ibl->load(res.ibl_path.c_str()))
            {
                fprintf_s(stderr, "Error : IBL texture load failed. path = %s\n", res.ibl_path.c_str());
            }
        }
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

        res.default_scene();
        cereal::XMLOutputArchive arc(stream);
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

Vector3 Scene::sample_ibl(const Vector3& dir) const
{ return m_ibl->sample3d(dir); }
