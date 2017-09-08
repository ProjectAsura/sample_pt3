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

bool median_split( size_t count, Triangle** tris, Box& box, size_t& mid, size_t& cnt0, size_t& cnt1 )
{
    // 最小要素に満たないものは葉ノードとして生成.  2つのノードがそれぞれ子供をもつので 2 * 2 = 4 が最小.
    if ( count <= 4 )
    { return false; }

    box = create_box( count, tris );

    // 分割軸を決めるためバウンディングボックスの最長軸を取得.
    auto axis = longest_axis( box );

    if (box.maxi.a[axis] == box.mini.a[axis])
    { return false; }

    mid = count / 2;
    std::nth_element(
        &tris[0],
        &tris[mid],
        &tris[count - 1] + 1,
        [axis](const Triangle* lhs, const Triangle* rhs)
        { return lhs->center().a[axis] < rhs->center().a[axis]; }
    );

    cnt0 = mid;
    cnt1 = count - mid;

    return true;
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

BVH::BVH( size_t count, Triangle** tris, const Box& box )
: m_tris(tris, count)
{
    m_node[0] = nullptr;
    m_node[1] = nullptr;
    m_box     = box;
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

void BVH::dispose()
{ delete this; }

BVH* BVH::build(std::vector<Triangle*>& tris)
{ return build_sub(uint32_t(tris.size()), tris.data()); }

BVH* BVH::build_sub(size_t count, Triangle** tris)
{
    size_t cnt0, cnt1, mid;
    Box box;

    if ( !median_split(count, tris, box, mid, cnt0, cnt1) )
    { return new BVH(count, tris, box); }

    // 再帰呼び出し.
    return new BVH(
        BVH::build_sub(cnt0, &tris[0]),
        BVH::build_sub(cnt1, &tris[mid]),
        box);
}

bool BVH::intersect(const Ray& ray, HitRecord& record) const
{
    // Boxと判定.
    if (!hit(ray, m_box))
    { return false; }

    auto hit = false;
    if (!m_tris.empty())
    {
        for(size_t j=0; j<m_tris.size(); ++j)
        { hit |= m_tris[j]->hit(ray, record); }

        return hit;
    }

    for(int i=0; i<2; ++i)
    { hit |= m_node[i]->intersect(ray, record); }

    return hit;
}


#if defined(ENABLE_SSE2)
///////////////////////////////////////////////////////////////////////////////////////////////////
// BVH4 class
///////////////////////////////////////////////////////////////////////////////////////////////////
BVH4::BVH4( BVH4* node0,  BVH4* node1, BVH4* node2, BVH4* node3, const Box4& box )
{
    m_node[0] = node0;
    m_node[1] = node1;
    m_node[2] = node2;
    m_node[3] = node3;
    m_box     = box;
}

BVH4::BVH4(size_t count, Triangle** tris, const Box& box)
: m_tris(tris, count)
{
    for(auto i=0; i<4; ++i)
    { m_node[i] = nullptr; }

    m_box = Box4( box );
}

BVH4::~BVH4()
{
    for(auto i=0; i<4; ++i)
    {
        if (m_node[i] != nullptr)
        {
            delete m_node[i];
            m_node[i] = nullptr;
        }
    }
}

void BVH4::dispose()
{ delete this; }

bool BVH4::intersect(const Ray& ray, HitRecord& record) const
{
    Ray4 ray4 = convert(ray);
    return intersect_sub(ray4, record);
}

bool BVH4::intersect_sub(const Ray4& ray4, HitRecord& record) const
{
    int mask = 0;

    // Boxと判定.
    if (!hit(ray4, m_box, mask))
    { return false; }

    auto hit = false;
    if (!m_tris.empty())
    {
        Ray ray = revert(ray4);

        for(size_t j=0; j<m_tris.size(); ++j)
        { hit |= m_tris[j]->hit(ray, record); }

        return hit;
    }

    for(int i=0; i<4; ++i)
    {
        auto bit = 0x1 << i;
        if ( (mask & bit) == bit)
        { hit |= m_node[i]->intersect_sub(ray4, record); }
    }

    return hit;
}

BVH4* BVH4::build(std::vector<Triangle*>& tris)
{ return build_sub(tris.size(), tris.data()); }

BVH4* BVH4::build_sub(size_t count, Triangle** tris)
{
    size_t cnt0, cnt1, mid;
    Box box;

    if (count <= 16)
    {
        box = create_box(count, tris);
        return new BVH4(count, tris, box);
    }

    if (!median_split(count, tris, box, mid, cnt0, cnt1))
    { return new BVH4(count, tris, box); }

    size_t cntA, cntB, cntC, cntD, midL, midR;
    Box boxL, boxR;

    if (!median_split(cnt0, &tris[0], boxL, midL, cntA, cntB))
    { return new BVH4(count, tris, box); }

    if (!median_split(cnt1, &tris[mid], boxR, midR, cntC, cntD))
    { return new BVH4(count, tris, box); }

    return new BVH4(
        build_sub(cntA, &tris[0]),
        build_sub(cntB, &tris[midL]),
        build_sub(cntC, &tris[mid]),
        build_sub(cntD, &tris[mid + midR]),
        Box4(box));
}

void* BVH4::operator new (size_t size)
{ return _aligned_malloc(size, 16); }

void* BVH4::operator new[] (size_t size)
{ return _aligned_malloc(size, 16); }

void BVH4::operator delete (void* ptr)
{ _aligned_free(ptr); }

void BVH4::operator delete[] (void* ptr)
{ _aligned_free(ptr); }

#endif//defined(ENABLE_SSE2)

#if defined(ENABLE_AVX)
///////////////////////////////////////////////////////////////////////////////////////////////////
// BVH8 class
///////////////////////////////////////////////////////////////////////////////////////////////////
BVH8::BVH8
(
    BVH8* n0, BVH8* n1, BVH8* n2, BVH8* n3,
    BVH8* n4, BVH8* n5, BVH8* n6, BVH8* n7,
    const Box8& box
)
{
    m_node[0] = n0;
    m_node[1] = n1;
    m_node[2] = n2;
    m_node[3] = n3;
    m_node[4] = n4;
    m_node[5] = n5;
    m_node[6] = n6;
    m_node[7] = n7;
    m_box     = box;
}

BVH8::BVH8(size_t count, Triangle** tris, const Box& box)
: m_tris(tris, count)
{
    for(auto i=0; i<8; ++i)
    { m_node[i] = nullptr; }

    m_box = Box8( box );
}

BVH8::~BVH8()
{
    for(auto i=0; i<8; ++i)
    {
        if (m_node[i] != nullptr)
        {
            delete m_node[i];
            m_node[i] = nullptr;
        }
    }
}

void BVH8::dispose()
{ delete this; }

bool BVH8::intersect(const Ray& ray, HitRecord& record) const
{
    Ray8 ray8 = make_ray8(ray);
    return intersect_sub(ray8, record);
}

bool BVH8::intersect_sub(const Ray8& ray8, HitRecord& record) const
{
    int mask = 0;

    // Boxと判定.
    if (!hit(ray8, m_box, mask))
    { return false; }

    auto hit = false;
    if (!m_tris.empty())
    {
        Ray ray = revert(ray8);

        for(size_t j=0; j<m_tris.size(); ++j)
        { hit |= m_tris[j]->hit(ray, record); }

        return hit;
    }

    for(int i=0; i<8; ++i)
    {
        auto bit = 0x1 << i;
        if ( (mask & bit) == bit)
        { hit |= m_node[i]->intersect_sub(ray8, record); }
    }

    return hit;
}

BVH8* BVH8::build(std::vector<Triangle*>& tris)
{ return build_sub(tris.size(), tris.data()); }

BVH8* BVH8::build_sub(size_t count, Triangle** tris)
{
    size_t cntL, cntR, mid;
    Box box;

    if (count <= 64)
    {
        box = create_box(count, tris);
        return new BVH8(count, tris, box);
    }

    if (!median_split(count, tris, box, mid, cntL, cntR))
    { return new BVH8(count, tris, box); }

    size_t cntA, cntB, cntC, cntD, midL, midR;
    Box boxL, boxR;

    if (!median_split(cntL, &tris[0], boxL, midL, cntA, cntB))
    { return new BVH8(count, tris, box); }

    if (!median_split(cntR, &tris[mid], boxR, midR, cntC, cntD))
    { return new BVH8(count, tris, box); }

    size_t cnt0, cnt1, cnt2, cnt3, cnt4, cnt5, cnt6, cnt7;
    size_t midA, midB, midC, midD;
    Box    boxA, boxB, boxC, boxD;

    if (!median_split(cntA, &tris[0], boxA, midA, cnt0, cnt1))
    { return new BVH8(count, tris, box); }

    if (!median_split(cntB, &tris[midL], boxB, midB, cnt2, cnt3))
    { return new BVH8(count, tris, box); }

    if (!median_split(cntC, &tris[mid], boxC, midC, cnt4, cnt5))
    { return new BVH8(count, tris, box); }

    if (!median_split(cntD, &tris[mid + midR], boxD, midD, cnt6, cnt7))
    { return new BVH8(count, tris, box); }

    auto idx0 = 0;
    auto idx1 = midA;
    auto idx2 = midL;
    auto idx3 = midL + midB;
    auto idx4 = mid;
    auto idx5 = mid + midC;
    auto idx6 = mid + midR;
    auto idx7 = mid + midR + midD;

    return new BVH8(
        build_sub(cnt0, &tris[idx0]),
        build_sub(cnt1, &tris[idx1]),
        build_sub(cnt2, &tris[idx2]),
        build_sub(cnt3, &tris[idx3]),
        build_sub(cnt4, &tris[idx4]),
        build_sub(cnt5, &tris[idx5]),
        build_sub(cnt6, &tris[idx6]),
        build_sub(cnt7, &tris[idx7]),
        Box8(box));
}

void* BVH8::operator new (size_t size)
{ return _aligned_malloc(size, 32); }

void* BVH8::operator new[] (size_t size)
{ return _aligned_malloc(size, 32); }

void BVH8::operator delete (void* ptr)
{ _aligned_free(ptr); }

void BVH8::operator delete[] (void* ptr)
{ _aligned_free(ptr); }

#endif//defined(ENABLE_AVX)
