//-------------------------------------------------------------------------------------------------
// File : r3d_cas.h
// Desc : Compare And Swap.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------
#pragma once


bool cas(void* volatile* ptr, void* compare, void* swap);
