#pragma once

#include "ECS/ComponentT.hpp"
extern "C"
{
#include "v2/_interface/BoxC_def.h"
#include "Objects/Resource/Mesh_def.h"
}

typedef struct RenderV2Interface_TYP RenderV2Interface;

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_ECS::ComponentHeaderT<MeshRenderer> ComponentHeader;
		RenderV2Interface* RenderInterface;
		MeshResource_HANDLE MeshResource;
		BoxF MeshBoundingBox;
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

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, RenderV2Interface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo);
}