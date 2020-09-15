#pragma once

#include "RenderHeap_def.h"

#include "Memory/PoolAllocator.h"

POOLALLOCATOR_ALLOC_FUNCTION(Vertex, PoolAllocator_Vertex_PTR, Vertex);
POOLALLOCATOR_ALLOCELEMENT_FUNCTION(Vertex, PoolAllocator_Vertex_PTR, Vertex, Vertex_HANDLE_PTR);
POOLALLOCATOR_FREEELEMENT_FUNCTION(Vertex, PoolAllocator_Vertex_PTR, Vertex_HANDLE);
POOLALLOCATOR_FREEELEMENTS_FUNCTION(Vertex, PoolAllocator_Vertex_PTR, Array_VertexHANDLE_PTR);

POOLALLOCATOR_ALLOC_FUNCTION(Polygon, PoolAllocator_Polygon_PTR, Polygon_VertexIndex);
POOLALLOCATOR_ALLOCELEMENT_FUNCTION(Polygon, PoolAllocator_Polygon_PTR, Polygon_VertexIndex, Polygon_VertexIndex_HANDLE_PTR);
POOLALLOCATOR_FREEELEMENT_FUNCTION(Polygon, PoolAllocator_Polygon_PTR, Polygon_VertexIndex_HANDLE);
POOLALLOCATOR_FREEELEMENTS_FUNCTION(Polygon, PoolAllocator_Polygon_PTR, Array_Polygon_VertexIndex_HANDLE_PTR);

POOLALLOCATOR_ALLOC_FUNCTION(Material, PoolAllocator_Material_PTR, Material);
POOLALLOCATOR_ALLOCELEMENT_FUNCTION(Material, PoolAllocator_Material_PTR, Material, Material_HANDLE_PTR);
POOLALLOCATOR_FREEELEMENT_FUNCTION(Material, PoolAllocator_Material_PTR, Material_HANDLE);


void RenderHeap_Alloc(RenderHeap_PTR p_renderHeap);
void RenderHeap_Free(RenderHeap_PTR p_renderHeap);
