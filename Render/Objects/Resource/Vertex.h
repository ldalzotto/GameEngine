#pragma once

#include "Vertex_def.h"
#include "DataStructures/ARRAY.h"
#include "Error/ErrorHandler.h"

inline void Arr_Alloc_Vertex(Array_Vertex_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc(&p_arr->array, sizeof(Vertex), p_initialCapacity); };
inline void Arr_Free_Vertex(Array_Vertex_PTR p_arr) { Arr_Free(&p_arr->array); };
inline void Arr_PushBackRealloc_Vertex(Array_Vertex_PTR p_arr, Vertex_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc(&p_arr->array, sizeof(Vertex), (char*)p_value)); };

ARRAY_ALLOC_FUNCTION(VertexHANDLE, Array_VertexHANDLE_PTR, Vertex_HANDLE);
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(VertexHANDLE, Array_VertexHANDLE_PTR, Vertex_HANDLE);