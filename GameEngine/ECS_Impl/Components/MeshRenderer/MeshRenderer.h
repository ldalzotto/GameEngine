#pragma once

#include "ECS/Component.h"
#include "glm/glm.hpp"

#include "Render/LoopStep/DefaultMaterialDrawStep.h"
#include "Render/Materials/DefaultMaterialV2Instance.h"
#include "Render/Mesh/Mesh.h"
#include "Render/Texture/Texture.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
	struct DefaultMaterialV2;
	struct DefaultMaterialDrawStep;
	struct TextureSamplers;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRendererDependencies
	{
		_Render::Device* Device;
		_Render::PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
		_Render::DefaultMaterialV2* DefaultMaterialV2;
		_Render::DefaultMaterialDrawStep* DefaultMaterialDrawStep;
	};

	struct MeshRenderer
	{
		MeshRendererDependencies MeshRendererDependencies;

		_Render::Mesh Mesh;
		_Render::Texture Texture;

		_Render::DefaultMaterialV2Instance DefaultMaterialV2Instance;
		_Utils::Subject OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		MeshRendererDependencies MeshRendererDependencies;
		_Render::TextureSamplers* TextureSamplers;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo);
	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection* l_meshUniformObject);
}