#pragma once

#include "Vertex_def.h"
#include "DataStructures/ARRAY.h"
#include "Error/ErrorHandler.h"

inline void Arr_Alloc_Vertex(Array_Vertex_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_arr, sizeof(Vertex), p_initialCapacity); };
inline void Arr_Free_Vertex(Array_Vertex_PTR p_arr) { Arr_Free((Array_PTR)p_arr); };
inline void Arr_PushBackRealloc_Vertex(Array_Vertex_PTR p_arr, Vertex_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((Array_PTR)p_arr, sizeof(Vertex), (char*)p_value)); };