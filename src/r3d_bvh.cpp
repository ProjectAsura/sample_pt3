//-------------------------------------------------------------------------------------------------
// File : r3d_bvh.cpp
// Desc : Bounding Volume Hierarchy.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_bvh.h>
#include <algorithm>


namespace {

//-------------------------------------------------------------------------------------------------
// Constant Values.
//-------------------------------------------------------------------------------------------------
constexpr int BucketCount = 12;     //!< バケット数です.

///////////////////////////////////////////////////////////////////////////////////////////////////
// Bucket structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Bucket
{
    size_t      count = 0;
    Box         box   = {};
};

Box create_box(size_t count, Triangle** tris)
{
    assert(count != 0);
    assert(tris != nullptr);

    Box result = tris[0]->box();

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

int longest_axis(const Box& box)
{
    auto dif = abs(box.maxi - box.mini);
    
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

} // namespace


///////////////////////////////////////////////////////////////////////////////////////////////////
// BVH class
///////////////////////////////////////////////////////////////////////////////////////////////////
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
    // 最小要素に満たないものは葉ノードとして生成.  2つのノードがそれぞれ子供をもつので 2 * 2 = 4 が最小.
    if ( count <= 4 )
    { return new BVH(count, tris); }

    auto bound = create_box( count, tris );

    // 分割軸を決めるためバウンディングボックスの最長軸を取得.
    auto axis = longest_axis( bound );

    if (bound.maxi.a[axis] == bound.mini.a[axis])
    { return new BVH(count, tris); }

    auto mid = (count) / 2;
    std::nth_element(
        &tris[0],
        &tris[mid],
        &tris[count - 1] + 1,
        [axis](const Triangle* lhs, const Triangle* rhs)
        { return lhs->center().a[axis] < rhs->center().a[axis]; }
    );

    size_t cnt0 = mid;
    size_t cnt1 = count - mid;

    // 再帰呼び出し.
    return new BVH(
        BVH::build_sub(cnt0, &tris[0]),
        BVH::build_sub(cnt1, &tris[mid]),
        bound);
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