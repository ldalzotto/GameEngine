#pragma once

#include "ECS/ComponentT.hpp"

#include "Materials/Material.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct ModelProjection;
	struct MaterialInstance;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_Render::RenderInterface* RenderInterface;
		
		_Render::MaterialInstance* MaterialInstance;
		_Render::MaterialUniqueKey MaterialUniqueKey;
	};

	template <>
	inline ComponentType* ComponentT_getComponentType<MeshRenderer>()
	{
		return &MeshRendererType;
	};

	struct MeshRendererInitInfo
	{
		_Render::MaterialUniqueKey* MaterialUniqueKey;
		std::unordered_map<std::string, void*> InputParameters;
	};

	void MeshRenderer_init(MeshRenderer** p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo);
}