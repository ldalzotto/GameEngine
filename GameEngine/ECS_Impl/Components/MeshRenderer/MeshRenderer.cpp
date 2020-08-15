#include "MeshRenderer.h"

#include "v2/Box/BoxMath.h"
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
		_MathV2::Box_build(&p_meshRenderer->MeshBoundingBox, (_Core::VectorT<_MathV2::Vector3<float>>*)&p_meshRenderer->MeshResource->Mesh.Vertices);
		p_meshRenderer->ComponentHeader.OnComponentFree = MeshRenderer_free;
	};

	void MeshRenderer_free(MeshRenderer* p_meshRenderer, ECS*)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Core::ResourceProviderT_releaseResource(l_meshRenderer->RenderInterface->Resources.MeshResourceProvider, _RenderV2::MeshResourceKey_getHashCode(&l_meshRenderer->MeshResource->Key));
	};
}