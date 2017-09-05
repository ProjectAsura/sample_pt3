//-------------------------------------------------------------------------------------------------
// File : r3d_shape.cpp
// Desc : Shape.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_shape.h>


namespace {


} // namespace

#if 0
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

bool Mesh::shadow_hit(const Ray& ray, float& dist) const
{
    return false;
}

bool Mesh::hit(const Ray& ray, HitRecord& record) const
{
    return false;
}

bool Mesh::load(const char* filename)
{
    return false;
}
#endif