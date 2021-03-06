﻿//-------------------------------------------------------------------------------------------------
// File : r3d_scene.h
// Desc : Scene
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_material.h>
#include <r3d_shape.h>
#include <r3d_camera.h>
#include <r3d_texture.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Scene class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Scene
{
public:
    Scene();
    ~Scene();

    bool load(const char* filename);
    bool save(const char* filename);
    void dispose();
    Ray  emit(float x, float y) const;
    bool hit(const Ray& ray, HitRecord& record) const;
    Vector3 sample_ibl(const Vector3& dir) const;

    int width  () const { return m_w; }
    int height () const { return m_h; }
    int samples() const { return m_s; }

private:
    int                     m_w;
    int                     m_h;
    int                     m_s;
    std::vector<Texture*>   m_texs;
    std::vector<Shape*>     m_objs;
    std::vector<Material*>  m_mats;
    Camera*                 m_cam;
    Texture*                m_ibl;
};
