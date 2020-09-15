#include "MeshRenderer.h"

#include "Objects/Resource/Mesh.h"
#include "Heap/RenderHeap.h"
#include "Objects/Resource/MeshResourceProviderV2.h"
#include "RenderV2Interface.h"

void MeshRenderer_init(MeshRenderer_PTR p_meshRenderer, RenderV2Interface* p_renderInterface, MeshRendererInitInfo_PTR p_mehsRendererInfo)
{
	MeshResourceProvider_UseResource(p_renderInterface->Resources.MeshResourceProvider, p_mehsRendererInfo->MeshResourcePath, &p_meshRenderer->MeshResource);
	Mesh_BuildBoundingBox(&p_meshRenderer->MeshResource->Mesh, &p_meshRenderer->MeshBoundingBox);
	PoolAllocator_AllocElement_Material(&RRenderHeap.MaterialAllocator, &p_meshRenderer->Material);
	RRenderHeap.MaterialAllocator.array.Memory[p_meshRenderer->Material.Handle] = p_mehsRendererInfo->Material;
};

void MeshRenderer_OnComponentDetached(MeshRenderer* p_meshRenderer, RenderV2Interface* p_renderInterface)
{
	MeshResourceProvider_ReleaseResource(p_renderInterface->Resources.MeshResourceProvider, &p_meshRenderer->MeshResource->Key);
	PoolAllocator_FreeElement_Material(&RRenderHeap.MaterialAllocator, p_meshRenderer->Material);
};