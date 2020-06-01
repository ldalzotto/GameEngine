#pragma once

#include "ECS/Component.h"
#include "glm/glm.hpp"

#include "Render/LoopStep/DefaultMaterialDrawStep.h"
#include "Render/Materials/DefaultMaterialV2Instance.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct DefaultMaterialV2;
	struct DefaultMaterialDrawStep;
	struct TextureResourceProvider;
	struct MeshResourceProvider;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRendererDependencies
	{
		_Render::Device* Device;
		_Render::DefaultMaterialV2* DefaultMaterialV2;
		_Render::DefaultMaterialDrawStep* DefaultMaterialDrawStep;
		_Render::TextureResourceProvider* TextureResourceProvider;
		_Render::MeshResourceProvider* MeshResourceProvider;
	};

	struct MeshRenderer
	{
		MeshRendererDependencies MeshRendererDependencies;

		_Render::DefaultMaterialV2Instance DefaultMaterialV2Instance;
		_Utils::Subject OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		MeshRendererDependencies MeshRendererDependencies;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo);
	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection* l_meshUniformObject);
}