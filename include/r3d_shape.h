//-------------------------------------------------------------------------------------------------
// File : r3d_shape.h
// Desc : Shapes.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>
#include <new>
#include <vector>


//-------------------------------------------------------------------------------------------------
// Forward Declaratiosn.
//-------------------------------------------------------------------------------------------------
struct Material;
struct Shape;
struct BVH4;
struct BVH8;


///////////////////////////////////////////////////////////////////////////////////////////////////
// Vertex structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Vertex
{
    Vector3 pos;
    Vector3 nrm;
    Vector2 uv;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// HitRecord structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct HitRecord
{
    float           dist    = F_HIT_MAX;                    // 衝突点までの距離.
    Vector3         pos     = Vector3(0.0f, 0.0f, 0.0f);    // 衝突点の位置座標.
    Vector3         nrm     = Vector3(0.0f, 0.0f, 0.0f);    // 法線ベクトル.
    Vector2         uv      = Vector2(0.0f, 0.0f);          // 衝突点のテクスチャ座標.
    const Shape*    shape   = nullptr;                      // 形状データ.
    const Material* mat     = nullptr;                      // 材質データ.
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// ShadowRecord structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct ShadowRecord
{
    float           dist  = F_HIT_MAX;
    float           pdf   = 1.0f;
    const Shape*    shape = nullptr;
    const Material* mat   = nullptr;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Shape structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct Shape
{
    virtual ~Shape() {}
    virtual bool hit(const Ray& ray, HitRecord& record) const = 0;
    virtual bool shadow_hit(const Ray& ray, ShadowRecord& record) const = 0;
    virtual void sample(Random& random, Vector3& pos, Vector3& nrm) const = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Sphere class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Sphere : public Shape
{
public:
    float           radius;     //!< 半径です.
    Vector3         pos;        //!< 位置座標です.
    const Material* mat;        //!< マテリアル.

    static Sphere* create(float radius, const Vector3& pos, const Material* mat)
    {
        auto instance = new (std::nothrow) Sphere();
        instance->radius = radius;
        instance->pos    = pos;
        instance->mat    = mat;
        return instance;
    }

    inline bool shadow_hit(const Ray& ray,ShadowRecord& record) const override
    {
        auto p = pos - ray.pos;
        auto b = dot(p, ray.dir);
        auto det = b * b - dot(p, p) + radius * radius;
        if (det < 0.0f)
        { return false; }

        auto sqrt_det = sqrt(det);
        auto t1 = b - sqrt_det;
        auto t2 = b + sqrt_det;
        if (t1 < F_HIT_MIN && t2 < F_HIT_MIN)
        { return false; }

        auto dist = ( t1 > F_HIT_MIN ) ? t1 : t2;
        if ( dist > record.dist )
        { return false; }

        record.dist  = dist;
        record.mat   = mat;
        record.pdf   = 1.0f / (4.0f * F_PI * radius * radius);
        record.shape = this;
 
        return true;
    }

    inline bool hit(const Ray& ray, HitRecord& record) const override
    {
        auto p = pos - ray.pos;
        auto b = dot(p, ray.dir);
        auto det = b * b - dot(p, p) + radius * radius;
        if (det < 0.0f)
        { return false; }

        auto sqrt_det = sqrt(det);
        auto t1 = b - sqrt_det;
        auto t2 = b + sqrt_det;
        if (t1 < F_HIT_MIN && t2 < F_HIT_MIN)
        { return false; }

        auto dist = ( t1 > F_HIT_MIN ) ? t1 : t2;
        if ( dist > record.dist )
        { return false; }

        record.dist  = dist;
        record.pos   = ray.pos + ray.dir * dist;
        record.nrm   = normalize(record.pos - pos);

        auto theta = acosf( record.nrm.y );
        auto phi   = atan2f( record.nrm.x, record.nrm.z );
        if ( phi < 0.0f )
        { phi += F_2PI; }

        record.uv    = Vector2( phi * F_1DIV2PI, ( F_PI - theta ) * F_1DIVPI );
        record.shape = this;
        record.mat   = mat;

        return true;
    }

    void sample(Random& random, Vector3& sample_pos, Vector3& sample_nrm) const override
    {
        const auto r1 = F_2PI * random.get_as_float();
        const auto r2 = 1.0f - 2.0f * random.get_as_float();
        const auto r3 = sqrt(1.0f - r2 * r2);

        const auto dir = normalize(Vector3(
            r3 * cos(r1),
            r3 * sin(r1),
            r2
        ));

        sample_pos = pos + dir * (radius + F_HIT_MIN);
        sample_nrm = normalize(sample_pos - pos);
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// Triangle class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Triangle : public Shape
{
public:
    static Triangle* create(const Vertex* vtx, const Material* mat)
    {
        auto instance = new (std::nothrow) Triangle();
        instance->m_vtx     = vtx;
        instance->m_mat     = mat;
        instance->m_edge[0] = vtx[1].pos - vtx[0].pos;
        instance->m_edge[1] = vtx[2].pos - vtx[0].pos;
        instance->m_edge[2] = vtx[2].pos - vtx[1].pos;
        return instance;
    }

    inline bool shadow_hit(const Ray& ray, ShadowRecord& record) const override
    {
        auto s1  = cross( ray.dir, m_edge[1] );
        auto div = dot( s1, m_edge[0] );

        if ( fabs(div) <= F_EPSILON )
        { return false; }

        auto d = ray.pos - m_vtx[0].pos;
        auto beta = dot( d, s1 ) / div;
        if ( beta <= 0.0 || beta >= 1.0 )
        { return false; }

        auto s2 = cross( d, m_edge[0] );
        auto gamma = dot( ray.dir, s2 ) / div;
        if ( gamma <= 0.0 || ( beta + gamma ) >= 1.0 )
        { return false; }

        auto t = dot( m_edge[1], s2 ) / div;
        if ( t < F_HIT_MIN || t > F_HIT_MAX )
        { return false; }

        if ( t >= record.dist )
        { return false; }

        record.dist  = t;
        record.mat   = m_mat;
        record.shape = this;
        record.pdf   = 1.0f;

        return true;
    }

    inline bool hit(const Ray& ray, HitRecord& record) const override
    {
        auto s1  = cross( ray.dir, m_edge[1] );
        auto div = dot( s1, m_edge[0] );

        if ( fabs(div) <= F_EPSILON )
        { return false; }

        auto d = ray.pos - m_vtx[0].pos;
        auto beta = dot( d, s1 ) / div;
        if ( beta <= 0.0 || beta >= 1.0 )
        { return false; }

        auto s2 = cross( d, m_edge[0] );
        auto gamma = dot( ray.dir, s2 ) / div;
        if ( gamma <= 0.0 || ( beta + gamma ) >= 1.0 )
        { return false; }

        auto dist = dot( m_edge[1], s2 ) / div;
        if ( dist < F_HIT_MIN || dist > F_HIT_MAX )
        { return false; }

        if ( dist >= record.dist )
        { return false; }

        record.pos   = ray.pos + ray.dir * dist;
        record.dist  = dist;
        record.shape = this;
        record.mat   = m_mat;

        auto alpha = 1.0f - beta - gamma;
        record.nrm = normalize(Vector3(
            m_vtx[0].nrm.x * alpha + m_vtx[1].nrm.x * beta + m_vtx[2].nrm.x * gamma,
            m_vtx[0].nrm.y * alpha + m_vtx[1].nrm.y * beta + m_vtx[2].nrm.y * gamma,
            m_vtx[0].nrm.z * alpha + m_vtx[1].nrm.z * beta + m_vtx[2].nrm.z * gamma ));

        record.uv = Vector2(
            m_vtx[0].uv.x * alpha + m_vtx[1].uv.x * beta + m_vtx[2].uv.x * gamma,
            m_vtx[0].uv.y * alpha + m_vtx[1].uv.y * beta + m_vtx[2].uv.y * gamma );

        return true;
    }

    void sample(Random& random, Vector3& sample_pos, Vector3& sample_nrm) const override
    {
        auto a = random.get_as_float();
        auto b = random.get_as_float();
        if (a + b > 1.0)
        {
            a = 1.0f - a;
            b = 1.0f - b;
        }

        sample_pos = m_vtx[0].pos + m_edge[0] * a + m_edge[1] * b;
        sample_nrm = normalize(cross(m_edge[0], m_edge[1]));
    }

private:
    const Vertex*   m_vtx;
    const Material* m_mat;
    Vector3         m_edge[3];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// ShapeInstance class
///////////////////////////////////////////////////////////////////////////////////////////////////
class ShapeInstance : public Shape
{
public:
    static ShapeInstance* create(Shape* shape, const Matrix& world)
    {
        auto instance = new (std::nothrow) ShapeInstance();
        instance->m_shape       = shape;
        instance->m_world       = world;
        instance->m_inv_world   = invert(world);
        return instance;
    }

    inline bool shadow_hit(const Ray& ray, ShadowRecord& record) const override
    {
        auto pos = mul_coord ( ray.pos, m_inv_world );
        auto dir = mul_normal( ray.dir, m_inv_world );
        auto localRaySet = Ray( pos, normalize(dir) );

        if ( m_shape->shadow_hit( localRaySet, record ) )
        { return true; }

        return false;
    }

    inline bool hit(const Ray& ray, HitRecord& record) const override
    {
        auto pos = mul_coord ( ray.pos, m_inv_world );
        auto dir = mul_normal( ray.dir, m_inv_world );
        auto localRaySet = Ray( pos, normalize(dir) );

        if ( m_shape->hit( localRaySet, record ) )
        {
            record.pos = mul( record.pos, m_world );
            record.nrm = mul_normal( record.nrm, transpose( m_inv_world ) );
            return true;
        }

        return false;
    }

    void sample(Random& random, Vector3& sample_pos, Vector3& sample_nrm) const override
    {
        Vector3 pos;
        Vector3 nrm;
        m_shape->sample(random, pos, nrm);
        sample_pos = mul(pos, m_world);
        sample_nrm = mul_normal(nrm, m_inv_world);
    }

private:
    Shape* m_shape;
    Matrix m_world;
    Matrix m_inv_world;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// Mesh class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Mesh : public Shape
{
public:
    static Mesh* create(const char* filename);
    bool shadow_hit(const Ray& ray, ShadowRecord& record) const override;
    bool hit(const Ray& ray, HitRecord& record) const override;
    void sample(Random& random, Vector3& sample_pos, Vector3& sample_nrm) const override;

private:
    std::vector<Vertex>     m_vtxs;
    std::vector<Material*>  m_mats;
    std::vector<Triangle*>  m_tris;
    BVH8*                   m_bvh;

    bool load(const char* filename);
};
