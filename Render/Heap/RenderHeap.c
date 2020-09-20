#include "RenderHeap.h"

#include "Memory/PoolAllocator.h"

#define VERTEX_BUFFER_INITIALSIZE 5000
#define POLYGON_BUFFER_INITIALSIZE 3000
#define MATERIAL_BUFFER_INITIALSIZE 10

RenderHeap RRenderHeap = {0};

void RenderHeap_Alloc(RenderHeap_PTR p_renderHeap)
{
	PoolAllocator_Alloc_Vertex(&p_renderHeap->VertexAllocator, VERTEX_BUFFER_INITIALSIZE);
	PoolAllocator_Alloc_Polygon(&p_renderHeap->PolygonAllocator, POLYGON_BUFFER_INITIALSIZE);
	PoolAllocator_Alloc_Material(&p_renderHeap->MaterialAllocator, MATERIAL_BUFFER_INITIALSIZE);
	PoolAllocator_Alloc_Texture3c(&p_renderHeap->Texture3cAllocator, 10);
};

void RenderHeap_Free(RenderHeap_PTR p_renderHeap)
{
	PoolAllocator_Free(&p_renderHeap->VertexAllocator.allocator);
	PoolAllocator_Free(&p_renderHeap->PolygonAllocator.allocator);
	PoolAllocator_Free(&p_renderHeap->MaterialAllocator.allocator);
	PoolAllocator_Free(&p_renderHeap->Texture3cAllocator.allocator);
};