#pragma once

#include "Render/Render.h"
#include "Render/Mesh/Mesh.h"
#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	extern ComponentType MeshRendererType;

	struct MeshRenderer
	{
		_Render::Render* Render;
		_Render::Mesh Mesh;
		_Utils::Subject OnComponentDetached;
	};

	struct MeshRendererInitInfo
	{
		Component* AssociatedComponent;
		_Render::Render* Render;
	};

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo);
}