#pragma once

#include "ECS/ECS.h"

namespace _GameEngine::_Render
{
	struct Render;
}

namespace _GameEngine::_ECS
{

	struct MeshDrawSystem
	{
		ECS* ECS;
		_Render::Render* Render;
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, ECS* p_ecs, _Render::Render* p_render);
	void MeshDrawSystem_update(MeshDrawSystem* p_meshDrawSystem, float p_delta);
	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem);
}