//-------------------------------------------------------------------------------------------------
// File : r3d_bvh.cpp
// Desc : Bounding Volume Hierarchy.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_bvh.h>


//-------------------------------------------------------------------------------------------------
// Constant Values.
//-------------------------------------------------------------------------------------------------
constexpr int BucketCount = 12;     //!< バケット数です.

///////////////////////////////////////////////////////////////////////////////////////////////////
// Bucket structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Bucket
{
    size_t      count;
    Box         box;

    //---------------------------------------------------------------------------------------------
    //! @brief      コンストラクタです.
    //---------------------------------------------------------------------------------------------
    Bucket()
    : count (0)
    , box   ()
    { /* DO_NOTHING */ }
};

Box create_box(size_t count, Triangle** tris)
{
    assert(count == 0);
    assert(tris == nullptr);

    Box result;
    result.mini = tris[0]->vertex(0).pos;
    result.maxi = tris[0]->vertex(0).pos;

    result.mini = min(result.mini, tris[0]->vertex(1).pos);
    result.maxi = max(result.maxi, tris[0]->vertex(1).pos);

    result.mini = min(result.mini, tris[0]->vertex(2).pos);
    result.maxi = max(result.maxi, tris[0]->vertex(2).pos);

    for(size_t i=1; i<count; ++i)
    {
        for(int j=0; j<3; ++j)
        {
            result.mini = min(result.mini, tris[i]->vertex(j).pos);
            result.maxi = max(result.maxi, tris[i]->vertex(j).pos);
        }
    }

    return result;
}

int longes_axis(const Box& box)
{
    auto dif = box.maxi - box.mini;
    
    if (dif.x > dif.y && dif.x > dif.z)
    { return 0; }
    else if (dif.y > dif.z)
    { return 1; }
    else
    { return 2; }
}

Vector3 calc_offset(const Box& box, const Vector3& p)
{
    auto offset = p - box.mini;

    if (box.maxi.x > box.mini.x)
    { offset.x /= box.maxi.x - box.mini.x; }

    if (box.maxi.y > box.mini.y)
    { offset.y /= box.maxi.y - box.mini.y; }

    if (box.maxi.z > box.mini.z)
    { offset.z /= box.maxi.z - box.mini.z; }

    return offset;
}

BVH::BVH(BVH* lhs, BVH* rhs, const Box& box)
{
    m_node[0] = lhs;
    m_node[1] = rhs;
    m_box     = box;
}

BVH::BVH( size_t count, Triangle** tris )
: m_tris(tris, count)
{
    m_node[0] = nullptr;
    m_node[1] = nullptr;
    m_box     = create_box(count, tris);
}

BVH::~BVH()
{
    if (m_node[0] != nullptr)
    {
        delete m_node[0];
        m_node[0] = nullptr;
    }

    if (m_node[1] != nullptr)
    {
        delete m_node[1];
        m_node[1] = nullptr;
    }
}

BVH* BVH::build(std::vector<Triangle*>& tris)
{ return build_sub(uint32_t(tris.size()), tris.data()); }

BVH* BVH::build_sub(size_t count, Triangle** tris)
{

}

bool BVH::intersect(const Ray& ray, HitRecord& record) const
{
    // Boxと判定.
    if (!hit(ray, m_box))
    { return false; }

    auto hit = false;
    for(int i=0; i<2; ++i)
    {
        if (!m_tris.empty())
        {
            for(size_t i=0; i<m_tris.size(); ++i)
            { hit |= m_tris[i]->hit(ray, record); }
        }
        else
        { hit |= m_node[i]->intersect(ray, record); }
    }

    return hit;
}