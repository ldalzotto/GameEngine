#pragma once

#include "ECS/Component.h"
#include "glm/glm.hpp"

#include "Materials/MaterialInstance.h"
#include "Materials/Material.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct ModelProjection;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_Render::RenderInterface* RenderInterface;
		
		_Render::MaterialInstance MaterialInstance;
		_Render::MaterialUniqueKey MaterialUniqueKey;

		_Utils::Subject OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		_Render::MaterialUniqueKey* MaterialUniqueKey;
		std::unordered_map<std::string, void*> InputParameters;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo);
}