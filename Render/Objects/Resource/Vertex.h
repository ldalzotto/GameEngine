#pragma once

#include "Vertex_def.h"
#include "_interface/DataStructures/ARRAY.h"
#include "_interface/Error/ErrorHandler.h"

inline void Arr_Alloc_Vertex(ARRAY_VERTEX_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc((ARRAY_PTR)p_arr, sizeof(VERTEX), p_initialCapacity); };
inline void Arr_Free_Vertex(ARRAY_VERTEX_PTR p_arr) { Arr_Free((ARRAY_PTR)p_arr); };
inline void Arr_PushBackRealloc_Vertex(ARRAY_VERTEX_PTR p_arr, VERTEX_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((ARRAY_PTR)p_arr, sizeof(VERTEX), (char*)p_value)); };