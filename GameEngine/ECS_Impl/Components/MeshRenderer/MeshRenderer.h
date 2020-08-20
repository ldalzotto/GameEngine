#pragma once

#include "ECS/ComponentT.hpp"
extern "C"
{
#include "v2/_interface/BoxC_def.h"
#include "Objects/Resource/Mesh_def.h"
}

namespace _RenderV2
{
	struct RenderV2Interface;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_ECS::ComponentHeaderT<MeshRenderer> ComponentHeader;
		_RenderV2::RenderV2Interface* RenderInterface;
		MESHRESOURCE_HANDLE MeshResource;
		BOXF MeshBoundingBox;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<MeshRenderer>()
	{
		return &MeshRendererType;
	};

	struct MeshRendererInitInfo
	{
		char* MeshResourcePath;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _RenderV2::RenderV2Interface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo);
}