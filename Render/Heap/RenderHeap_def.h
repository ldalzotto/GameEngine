#pragma once

#include "Memory/PoolAllocator_def.h"
#include "Objects/Resource/Mesh_def.h"

typedef struct PoolAllocator_Vertex_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_Vertex)
}PoolAllocator_Vertex, * PoolAllocator_Vertex_PTR;

typedef struct PoolAllocator_Polygon_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_Polygon_VertexIndex)
}PoolAllocator_Polygon, * PoolAllocator_Polygon_PTR;

typedef struct RenderHeap_TYP
{
	PoolAllocator_Vertex VertexAllocator;
	PoolAllocator_Polygon PolygonAllocator;
}RenderHeap, * RenderHeap_PTR;

extern RenderHeap RRenderHeap;