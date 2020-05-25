#pragma once

#include "ECS/System.h"
#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	struct MeshDrawSystem
	{
		EntityComponentListener EntityComponentListener;
	};

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, ComponentEvents* p_componentEvents);
	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem, ComponentEvents* p_componentEvents);
}