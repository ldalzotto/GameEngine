#pragma once

#include "Mesh_def.h"

#include "_interface/Error/ErrorHandler.h"
#include "_interface/DataStructures/ARRAY.h"

void Mesh_Alloc(MESH_PTR p_mesh);
void Mesh_Free(MESH_PTR p_mesh);

inline void Arr_Alloc_Polygon_VertexIndex(ARRAY_POLYGON_VERTEXINDEX_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc((ARRAY_PTR)p_arr, sizeof(POLYGON_VERTEXINDEX), p_initialCapacity); };
inline void Arr_Free_Polygon_VertexIndex(ARRAY_POLYGON_VERTEXINDEX_PTR p_arr) { Arr_Free((ARRAY_PTR)p_arr); };
inline void Arr_PushBackRealloc_Polygon_VertexIndex(ARRAY_POLYGON_VERTEXINDEX_PTR p_arr, POLYGON_VERTEXINDEX_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((ARRAY_PTR)p_arr, sizeof(POLYGON_VERTEXINDEX), (char*)p_value)); };
