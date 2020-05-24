#pragma once

#include "Render/Render.h"
#include "Render/Mesh/Mesh.h"
#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	struct MeshRenderer
	{
		Component* Component;
		_Render::Render* Render;
		_Render::Mesh Mesh;
	};

	struct MeshRendererInitInfo
	{
		_Render::Render* Render;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo);
	void MeshRenderer_free(Component* p_meshRenderer);
}