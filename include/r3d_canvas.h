//-------------------------------------------------------------------------------------------------
// File : r3d_canvas.h
// Desc : Canvas Module.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_math.h>
#include <vector>
#include <mutex>


///////////////////////////////////////////////////////////////////////////////////////////////////
// Canvas class
///////////////////////////////////////////////////////////////////////////////////////////////////
class Canvas
{
public:
    Canvas();
    ~Canvas();

    void            resize  (int w, int h);
    Vector3&        at      (int x, int y);
    const Vector3&  at      (int x, int y) const;
    const Vector3*  data    () const;
    void            add     (int x, int y, const Vector3& value);

    bool write(const char* filename);
    bool write(int counter);

private:
    int                     m_w;
    int                     m_h;
    std::vector<Vector3>    m_pixels;
    std::vector<Vector3>    m_temps;
    std::vector<uint8_t>    m_output;

    void tonemap_none();
    void tonemap_reinhard();
    void tonemap_aces();
    void gamma_correction();
    void srgb_correction();

    void median_filter();
};

