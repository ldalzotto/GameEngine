#pragma once

#include "Memory/PoolAllocator_def.h"
#include "Objects/Resource/Mesh_def.h"

typedef struct PoolAllocator_Vertex_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_Vertex)
}PoolAllocator_Vertex, * PoolAllocator_Vertex_PTR;

typedef struct RenderHeap_TYP
{
	PoolAllocator_Vertex VertexAllocator;
}RenderHeap, * RenderHeap_PTR;

extern RenderHeap RRenderHeap;