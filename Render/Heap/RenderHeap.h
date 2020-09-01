#pragma once

#include "RenderHeap_def.h"

#include "Memory/PoolAllocator.h"

POOLALLOCATOR_ALLOC_FUNCTION(Vertex, PoolAllocator_Vertex_PTR, Vertex);
POOLALLOCATOR_ALLOCELEMENT_FUNCTION(Vertex, PoolAllocator_Vertex_PTR, Vertex, Vertex_HANDLE_PTR);
POOLALLOCATOR_FREEELEMENT_FUNCTION(Vertex, PoolAllocator_Vertex_PTR, Vertex_HANDLE);

void RenderHeap_Alloc(RenderHeap_PTR p_renderHeap);
void RenderHeap_Free(RenderHeap_PTR p_renderHeap);