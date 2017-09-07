//-------------------------------------------------------------------------------------------------
// File : r3d_array.h
// Desc : Reference Array.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <cassert>


///////////////////////////////////////////////////////////////////////////////////////////////////
// ref_array class
///////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class ref_array
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
    ref_array()
    : m_ptr(nullptr)
    , m_count(0)
    { /* DO_NOTHING */ }

    ref_array(T* ptr, size_t count)
    : m_ptr(ptr)
    , m_count(count)
    { /* DO_NOTHING */ }

    size_t size() const
    { return m_count; }

    ref_array<T> sub_array(int start, int end)
    {
        assert(start < m_count);
        assert(end   < m_count);
        return ref_array<T>(m_ptr + start, end - start);
    }

    T* begin()
    { return m_ptr; }

    T* end()
    { return m_ptr + m_count; }

    const T* begin() const
    { return m_ptr; }

    const T* end() const
    { return m_ptr + m_count; }

    T& at(size_t index)
    {
        assert(index < m_count);
        return m_ptr[index];
    }

    const T& at(size_t index) const
    {
        assert(index < m_count);
        return m_ptr[index];
    }

    T& operator[](size_t index)
    { return at(index); }

    const T& operator[](size_t index) const
    { return at(index); }

    bool empty() const
    { return m_ptr == nullptr || m_count == 0; }

private:
    //=============================================================================================
    // private variables.
    //=============================================================================================
    T*      m_ptr;
    size_t  m_count;

    //=============================================================================================
    // private methods.
    //=============================================================================================
    /* NOTHING */
};