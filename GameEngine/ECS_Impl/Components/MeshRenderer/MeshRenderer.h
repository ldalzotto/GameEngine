#pragma once

#include "ECS/Component.h"

#include "Materials/Material.h"

extern "C"
{
#include "Functional/Callback/Callback.h"
}

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

		Callback OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		_Render::MaterialUniqueKey* MaterialUniqueKey;
		std::unordered_map<std::string, void*> InputParameters;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo);
}