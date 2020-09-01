#include "MeshRenderer.h"

#include "v2/_interface/BoxC.h"
#include "v2/_interface/VectorStructuresC.h"
#include "Heap/RenderHeap.h"
#include "Objects/Resource/MeshResourceProviderV2.h"
#include "RenderV2Interface.h"

void MeshRenderer_init(MeshRenderer_PTR p_meshRenderer, RenderV2Interface* p_renderInterface, MeshRendererInitInfo_PTR p_mehsRendererInfo)
{
	MeshResourceProvider_UseResource(p_renderInterface->Resources.MeshResourceProvider, p_mehsRendererInfo->MeshResourcePath, &p_meshRenderer->MeshResource);
	

	Array_Vector3f l_vertices;
	Arr_Alloc_Vector3F(&l_vertices, p_meshRenderer->MeshResource->Mesh.Vertices.Size);
	for (size_t i = 0; i < p_meshRenderer->MeshResource->Mesh.Vertices.Size; i++)
	{
		Arr_PushBackNoRealloc_Vector3F(&l_vertices, (Vector3f_PTR)&RRenderHeap.VertexAllocator.array.Memory[p_meshRenderer->MeshResource->Mesh.Vertices.Memory[i].Handle].LocalPosition);
	}
	Box_Build_F(&p_meshRenderer->MeshBoundingBox, &l_vertices);
	Arr_Free_Vector3F(&l_vertices);
	// Array_VertexHANDLE l_vertices = { .Memory = p_meshRenderer->MeshResource->Mesh.Vertices.Memory, .Size = p_meshRenderer->MeshResource->Mesh.Vertices.Size, .Capacity = p_meshRenderer->MeshResource->Mesh.Vertices.Capacity };
	
	// Array_Vector3f_Iterator l_verticesIterator = {.Array = &p_meshRenderer->MeshResource->Mesh.Vertices, .Current = NULL, .CurrentIndex = -1, .ElementOffset = offsetof() };
};

/*
Array_PTR Array;
	char* Current;
	size_t CurrentIndex;
	size_t ElementOffset;
*/

void MeshRenderer_OnComponentDetached(MeshRenderer* p_meshRenderer, RenderV2Interface* p_renderInterface)
{
	MeshResourceProvider_ReleaseResource(p_renderInterface->Resources.MeshResourceProvider, &p_meshRenderer->MeshResource->Key);
};