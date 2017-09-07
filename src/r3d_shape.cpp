//-------------------------------------------------------------------------------------------------
// File : r3d_shape.cpp
// Desc : Shape.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_shape.h>
#include <r3d_texture.h>
#include <r3d_material.h>
#include <smd.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Mesh class
///////////////////////////////////////////////////////////////////////////////////////////////////
Mesh* Mesh::create(const char* filename)
{
    auto instance = new(std::nothrow) Mesh();
    if (!instance->load(filename))
    {
        delete instance;
        return nullptr;
    }

    return instance;
}


bool Mesh::hit(const Ray& ray, HitRecord& record) const
{
    bool hit = false;
    for(size_t i=0; i<m_tris.size(); ++i)
    {
        hit |= m_tris[i]->hit(ray, record);
    }

    return hit;
}

bool Mesh::load(const char* filename)
{
    FILE* file;

    auto err = fopen_s(&file, filename, "rb");
    if (err != 0)
    {
        fprintf_s(stderr, "Error : Mesh File Open Failed.path = %s\n", filename);
        return false;
    }

    SMD_FILE_HEADER header;
    fread(&header, sizeof(header), 1, file);

    if (memcmp(header.Magic, SMD_FILE_TAG, sizeof(uint8_t) * 4) != 0)
    {
        fprintf_s(stderr, "Error : Invalid Mesh File.\n");
        return false;
    }

    if (header.Version != SMD_CURRENT_VERSION)
    {
        fprintf_s(stderr, "Error : Invalid File Version.\n");
        return false;
    }

    m_vtxs.resize(header.VertexCount);
    m_mats.resize(header.MaterialCount);
    m_texs.resize(header.TextureCount);
    m_tris.resize(header.TriangleCount);

    for(uint32_t i=0; i<header.VertexCount; ++i)
    {
        SMD_VERTEX vert;
        fread(&vert, sizeof(vert), 1, file);

        m_vtxs[i].pos = vert.Position;
        m_vtxs[i].nrm = vert.Normal;
        m_vtxs[i].uv  = vert.Texcoord;
    }

    for(uint32_t i=0; i<header.TextureCount; ++i)
    {
        SMD_TEXTURE tex;
        fread(&tex, sizeof(tex), 1, file);

        m_texs[i] = new (std::nothrow) Texture();
        if (!m_texs[i]->load(tex.Path))
        {
            fprintf_s(stderr, "Error : Texture Load Failed. path = %s\n", tex.Path);
        }
    }

    for(uint32_t i=0; i<header.MaterialCount; ++i)
    {
        SMD_MATERIAL mat;
        fread(&mat, sizeof(mat), 1, file);

        switch(mat.Type)
        {
        case SMD_MATERIAL_TYPE_LAMBERT:
            {
                m_mats[i] = Lambert::create(mat.Color, mat.Emissive);
            }
            break;

        case SMD_MATERIAL_TYPE_MIRROR:
            {
                m_mats[i] = Mirror::create(mat.Color, mat.Emissive);
            }
            break;

        case SMD_MATERIAL_TYPE_REFRACT:
            {
                m_mats[i] = Refract::create(mat.Color, mat.Ior, mat.Emissive);
            }
            break;

        case SMD_MATERIAL_TYPE_PHONG:
            {
                m_mats[i] = Phong::create(mat.Color, mat.Shininess, mat.Emissive);
            }
            break;
        }
    }

    for(uint32_t i=0; i<header.TriangleCount; ++i)
    {
        SMD_TRIANGLE tri;
        fread(&tri, sizeof(tri), 1, file);

        m_tris[i] = Triangle::create(&m_vtxs[tri.VertexOffset], m_mats[tri.MaterialId]);
    }

    fclose(file);

    return true;
}
