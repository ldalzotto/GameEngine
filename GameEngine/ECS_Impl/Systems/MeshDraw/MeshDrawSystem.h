#pragma once

#include "ECS/ECS.h"

namespace _GameEngine::_ECS
{

	struct MeshDrawSystem
	{
		ECS* ECS;
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, ECS* p_ecs);
	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem);
}