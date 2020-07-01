#pragma once

#include "ECS/Component.h"

#include "Materials/Material.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct ModelProjection;
	struct MaterialInstance;
}

namespace _GameEngine::_ECS
{
	struct MeshRenderer
	{
		_Render::RenderInterface* RenderInterface;
		
		_Render::MaterialInstance* MaterialInstance;
		_Render::MaterialUniqueKey MaterialUniqueKey;

		_Utils::Subject OnComponentDetached;
	};

	template<>
	extern inline ComponentType* extractComponentType<MeshRenderer>();

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		_Render::MaterialUniqueKey* MaterialUniqueKey;
		std::unordered_map<std::string, void*> InputParameters;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo);
}