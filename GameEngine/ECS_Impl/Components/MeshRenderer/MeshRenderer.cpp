#include "MeshRenderer.h"

extern "C"
{
#include "v2/_interface/BoxC.h"
#include "Objects/Resource/MeshResourceProviderV2.h"
}
#include "RenderV2Interface.hpp"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(MeshRenderer* p_meshRenderer, ECS*);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _RenderV2::RenderV2Interface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->RenderInterface = p_renderInterface;

		MeshResourceProvider_UseResource(p_renderInterface->Resources.MeshResourceProvider, p_mehsRendererInfo->MeshResourcePath, &p_meshRenderer->MeshResource);

		ARRAY_VECTOR3F l_vertices = { .Memory = (VECTOR3F*)p_meshRenderer->MeshResource->Mesh.Vertices.Memory, .Size = p_meshRenderer->MeshResource->Mesh.Vertices.Size, .Capacity = p_meshRenderer->MeshResource->Mesh.Vertices.Capacity };
		Box_Build_F(&p_meshRenderer->MeshBoundingBox, &l_vertices);
		p_meshRenderer->ComponentHeader.OnComponentFree = MeshRenderer_free;
	};

	void MeshRenderer_free(MeshRenderer* p_meshRenderer, ECS*)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		MeshResourceProvider_ReleaseResource(l_meshRenderer->RenderInterface->Resources.MeshResourceProvider, &l_meshRenderer->MeshResource->Key);
	};
}