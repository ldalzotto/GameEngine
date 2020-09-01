#pragma once

#include "Mesh_def.h"

#include "Error/ErrorHandler.h"
#include "DataStructures/ARRAY.h"

void Mesh_Alloc(Mesh_PTR p_mesh);
void Mesh_AllocVertex(Mesh_PTR p_mesh, Vertex_HANDLE_PTR out_vertexHandle);
void Mesh_Free(Mesh_PTR p_mesh);

inline void Arr_Alloc_Polygon_VertexIndex(Array_Polygon_VertexIndex_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc(&p_arr->array, sizeof(Polygon_VertexIndex), p_initialCapacity); };
inline void Arr_Free_Polygon_VertexIndex(Array_Polygon_VertexIndex_PTR p_arr) { Arr_Free(&p_arr->array); };
inline void Arr_PushBackRealloc_Polygon_VertexIndex(Array_Polygon_VertexIndex_PTR p_arr, Polygon_VertexIndex_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc(&p_arr->array, sizeof(Polygon_VertexIndex), (char*)p_value)); };
