#pragma once

#include "ECS/Component.h"
#include "glm/glm.hpp"

#include "Render/Materials/DefaultMaterialV2Instance.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_Render::RenderInterface* RenderInterface;
		_Render::DefaultMaterialV2Instance DefaultMaterialV2Instance;
		_Utils::Subject OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		_Render::DefaultMaterialV2Instance_InputAssets* DefaultMaterialV2Instance_InputAssets;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, _Render::RenderInterface* p_renderInterface, MeshRendererInitInfo* p_mehsRendererInfo);
	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection* l_meshUniformObject);
}