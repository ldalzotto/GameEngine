#include "MeshRenderer.h"

extern "C"
{
#include "v2/_interface/BoxC.h"
}
#include "RenderV2Interface.hpp"
#include "Objects/Resource/MeshResourceProvider.hpp"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(MeshRenderer* p_meshRenderer, ECS*);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _RenderV2::RenderV2Interface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->RenderInterface = p_renderInterface;
		_RenderV2::MeshResourceKey l_meshResourceKey;
		_Core::String_alloc(&l_meshResourceKey.MeshPathAbsolute, 0); _Core::String_append(&l_meshResourceKey.MeshPathAbsolute, p_mehsRendererInfo->MeshResourcePath);
		p_meshRenderer->MeshResource = _Core::ResourceProviderT_useResource(p_renderInterface->Resources.MeshResourceProvider, &l_meshResourceKey, _RenderV2::MeshResourceKey_getHashCode(&l_meshResourceKey));
		_Core::String_free(&l_meshResourceKey.MeshPathAbsolute);

		ARRAY_VECTOR3F l_vertices = { .Memory = (VECTOR3F*)p_meshRenderer->MeshResource->Mesh.Vertices.Memory, .Size = p_meshRenderer->MeshResource->Mesh.Vertices.Size, .Capacity = p_meshRenderer->MeshResource->Mesh.Vertices.Capacity };
		Box_Build_F(&p_meshRenderer->MeshBoundingBox, &l_vertices);
		p_meshRenderer->ComponentHeader.OnComponentFree = MeshRenderer_free;
	};

	void MeshRenderer_free(MeshRenderer* p_meshRenderer, ECS*)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Core::ResourceProviderT_releaseResource(l_meshRenderer->RenderInterface->Resources.MeshResourceProvider, _RenderV2::MeshResourceKey_getHashCode(&l_meshRenderer->MeshResource->Key));
	};
}