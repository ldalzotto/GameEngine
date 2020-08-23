#include "MeshRenderer.h"

#include "v2/_interface/BoxC.h"
#include "Objects/Resource/MeshResourceProviderV2.h"
#include "RenderV2Interface.h"

void MeshRenderer_init(MeshRenderer_PTR p_meshRenderer, RenderV2Interface* p_renderInterface, MeshRendererInitInfo_PTR p_mehsRendererInfo)
{
	MeshResourceProvider_UseResource(p_renderInterface->Resources.MeshResourceProvider, p_mehsRendererInfo->MeshResourcePath, &p_meshRenderer->MeshResource);

	Array_Vector3f l_vertices = { .Memory = (Vector3f*)p_meshRenderer->MeshResource->Mesh.Vertices.Memory, .Size = p_meshRenderer->MeshResource->Mesh.Vertices.Size, .Capacity = p_meshRenderer->MeshResource->Mesh.Vertices.Capacity };
	Box_Build_F(&p_meshRenderer->MeshBoundingBox, &l_vertices);
};

void MeshRenderer_OnComponentDetached(MeshRenderer* p_meshRenderer, RenderV2Interface* p_renderInterface)
{
	MeshResourceProvider_ReleaseResource(p_renderInterface->Resources.MeshResourceProvider, &p_meshRenderer->MeshResource->Key);
};