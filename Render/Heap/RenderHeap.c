#include "RenderHeap.h"

#include "Memory/PoolAllocator.h"

#define VERTEX_BUFFER_INITIALSIZE 5000

RenderHeap RRenderHeap = {0};

void RenderHeap_Alloc(RenderHeap_PTR p_renderHeap)
{
	PoolAllocator_Alloc_Vertex(&p_renderHeap->VertexAllocator, VERTEX_BUFFER_INITIALSIZE);
};

void RenderHeap_Free(RenderHeap_PTR p_renderHeap)
{
	PoolAllocator_Free(&p_renderHeap->VertexAllocator.allocator);
};