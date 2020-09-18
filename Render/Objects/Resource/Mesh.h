#pragma once

#include "Mesh_def.h"

#include "Error/ErrorHandler.h"
#include "DataStructures/ARRAY.h"
#include "v2/_interface/BoxC_def.h"

void Mesh_Alloc(Mesh_PTR p_mesh);
void Mesh_AllocVertex(Mesh_PTR p_mesh, Vertex_HANDLE_PTR out_vertexHandle);
void Mesh_AllocPolygonUV(Mesh_PTR p_mesh, Polgyon_UV_HANDLE_PTR out_polygonUvhandle);
void Mesh_Free(Mesh_PTR p_mesh);
void Mesh_BuildBoundingBox(const Mesh_PTR p_mesh, BoxF_PTR out_box);

ARRAY_ALLOC_FUNCTION(Polygon_VertexIndex_HANDLE, Array_Polygon_VertexIndex_HANDLE_PTR, Polygon_VertexIndex_HANDLE);
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(Polygon_VertexIndex_HANDLE, Array_Polygon_VertexIndex_HANDLE_PTR, Polygon_VertexIndex_HANDLE);

ARRAY_ALLOC_FUNCTION(PolygonUVHandle, Array_PolygonUVHandle_PTR, Polgyon_UV_HANDLE);
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(PolygonUVHandle, Array_PolygonUVHandle_PTR, Polgyon_UV_HANDLE);
