﻿//-------------------------------------------------------------------------------------------------
// File : r3d_bvh.h
// Desc : Bounding Volume Hierarchy.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>
#include <r3d_shape.h>
#include <r3d_array.h>


///////////////////////////////////////////////////////////////////////////////////////////////////
// BVH class
///////////////////////////////////////////////////////////////////////////////////////////////////
class BVH
{
    //=============================================================================================
    // list of friend classes and methods.
    //=============================================================================================
    /* NOTHING */

public:
    //=============================================================================================
    // public variables.
    //=============================================================================================
    /* NOTHING */

    //=============================================================================================
    // public methods.
    //=============================================================================================
    BVH();
    ~BVH();

    static BVH* build(std::vector<Triangle*>& tris);
    bool intersect(const Ray& ray, HitRecord& record) const;

private:
    BVH*                    m_node[2];
    Box                     m_box;
    ref_array<Triangle*>    m_tris;

    //=============================================================================================
    // private methods.
    //=============================================================================================
    BVH( BVH* lhs, BVH* rhs, const Box& box );
    BVH( size_t count, Triangle** tris );
    static BVH* build_sub(size_t count, Triangle** tris);
};
