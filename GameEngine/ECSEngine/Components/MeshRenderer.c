#include "MeshRenderer.h"

#include "Asset/AssetPaths.h"
#include "Objects/Resource/Mesh.h"
#include "Objects/Resource/MeshResourceProviderV2.h"
#include "Heap/RenderHeap.h"
#include "RenderV2Interface.h"

void MeshRenderer_init(MeshRenderer_PTR p_meshRenderer, RenderV2Interface* p_renderInterface, MeshRendererInitInfo_PTR p_mehsRendererInfo)
{
	Assetpath l_meshAssetFullpath;
	AssetPath_GetAbsolutePath(p_mehsRendererInfo->MeshResourcePath, &l_meshAssetFullpath);
	MeshResourceProvider_UseResource(p_renderInterface->Resources.MeshResourceProvider, &l_meshAssetFullpath, &p_meshRenderer->MeshResource);
	Mesh_BuildBoundingBox(&p_meshRenderer->MeshResource->Mesh, &p_meshRenderer->MeshBoundingBox);
	PoolAllocator_AllocElement_Material(&RRenderHeap.MaterialAllocator, &p_meshRenderer->Material);
	RRenderHeap.MaterialAllocator.array.Memory[p_meshRenderer->Material.Handle] = p_mehsRendererInfo->Material;
};

void MeshRenderer_OnComponentDetached(MeshRenderer* p_meshRenderer, RenderV2Interface* p_renderInterface)
{
	MeshResourceProvider_ReleaseResource(p_renderInterface->Resources.MeshResourceProvider, &p_meshRenderer->MeshResource->Key);
	PoolAllocator_FreeElement_Material(&RRenderHeap.MaterialAllocator, p_meshRenderer->Material);
};