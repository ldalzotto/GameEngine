#pragma once

#include "ECS/Component.h"
#include "glm/glm.hpp"

#include "Render/LoopStep/DefaultMaterialDrawStep.h"
#include "Render/Mesh/Mesh.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderStagingStep;
	struct DefaultMaterial;
	struct DefaultMaterialDrawStep;
}

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRendererDependencies
	{
		_Render::Device* Device;
		_Render::PreRenderStagingStep* PreRenderStaggingStep;
		_Render::DefaultMaterial* DefaultMaterial;
		_Render::DefaultMaterialDrawStep* DefaultMaterialDrawStep;
	};

	struct MeshRenderer
	{
		MeshRendererDependencies MeshRendererDependencies;

		_Render::Mesh Mesh;
		_Render::DefaultMaterialDrawCommand DefaultMaterialDrawCommand;
		_Utils::Subject OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		MeshRendererDependencies MeshRendererDependencies;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo);
	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection& l_meshUniformObject);
}