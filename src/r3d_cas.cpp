//-------------------------------------------------------------------------------------------------
// File : r3d_cas.cpp
// Desc : Compare And Swap.
// Copyright(c) Project Asura.
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------------
#include <r3d_cas.h>
#include <cstdint>


#if defined(WIN32) || defined(_WIN32) || defined(WIN64)
/* Windows系*/

#include <Windows.h>

bool cas(void* volatile* ptr, void* compare, void* swap)
{ return InterlockedCompareExchangePointer(ptr, swap, compare) == compare; }

#else
/* Linux系　*/

bool cas(void* volatile* ptr, void* compare, void* swap)
{
    return __sync_bool_compare_and_swap(
        reinterpret_cast<volatile uint64_t*>(ptr),
        reinterpret_cast<uint64_t>(compare),
        reinterpret_cast<uint64_t>(swap));
}

#endif
