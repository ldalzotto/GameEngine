#pragma once

#include "ECS/ComponentT.hpp"
extern "C"
{
#include "v2/_interface/BoxC_def.h"
}

namespace _RenderV2
{
	struct RenderV2Interface;
	struct MeshResource;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_ECS::ComponentHeaderT<MeshRenderer> ComponentHeader;
		_RenderV2::RenderV2Interface* RenderInterface;
		_RenderV2::MeshResource* MeshResource;
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