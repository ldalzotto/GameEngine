#include "Mesh.h"

#include "Heap/RenderHeap.h"
#include "Vertex.h"

void Mesh_Alloc(Mesh_PTR p_mesh)
{
	Arr_Alloc_VertexHANDLE(&p_mesh->Vertices, 0);
	Arr_Alloc_Polygon_VertexIndex(&p_mesh->Polygons, 0);
};

void Mesh_AllocVertex(Mesh_PTR p_mesh, Vertex_HANDLE_PTR out_vertexHandle)
{
	PoolAllocator_AllocElement_Vertex(&RRenderHeap.VertexAllocator, out_vertexHandle);
	Arr_PushBackRealloc_VertexHANDLE(&p_mesh->Vertices, out_vertexHandle);
};

void Mesh_Free(Mesh_PTR p_mesh)
{
	PoolAllocator_FreeElements(&RRenderHeap.VertexAllocator.allocator, &p_mesh->Vertices);
	Arr_Free(&p_mesh->Vertices.array);
	Arr_Free_Polygon_VertexIndex(&p_mesh->Polygons);
};