#pragma once

#include "Render/Render.h"
#include "Render/Mesh/Mesh.h"
#include "ECS/Component.h"
#include "glm/glm.hpp"

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_Render::Render* Render;

		_Render::Mesh Mesh;
		_Render::MeshDrawCommand MeshDrawCommand;
		_Utils::Subject OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		_Render::Render* Render;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo);
	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::ModelProjection& l_meshUniformObject);
}