#pragma once

#include "Mesh_def.h"

#include "Error/ErrorHandler.h"
#include "DataStructures/ARRAY.h"

void Mesh_Alloc(Mesh_PTR p_mesh);
void Mesh_Free(Mesh_PTR p_mesh);

inline void Arr_Alloc_Polygon_VertexIndex(Array_Polygon_VertexIndex_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_arr, sizeof(Polygon_VertexIndex), p_initialCapacity); };
inline void Arr_Free_Polygon_VertexIndex(Array_Polygon_VertexIndex_PTR p_arr) { Arr_Free((Array_PTR)p_arr); };
inline void Arr_PushBackRealloc_Polygon_VertexIndex(Array_Polygon_VertexIndex_PTR p_arr, Polygon_VertexIndex_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((Array_PTR)p_arr, sizeof(Polygon_VertexIndex), (char*)p_value)); };
