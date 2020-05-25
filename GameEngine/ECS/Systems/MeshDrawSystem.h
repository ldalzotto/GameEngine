#pragma once

#include "ECS/System.h"
#include "ECS/Component.h"

namespace _GameEngine::_ECS
{
	struct MeshDrawSystem
	{
		EntityComponentListener EntityComponentListener;
	};

	struct MeshDrawSystemInitInfo
	{
		EntityContainer* EntityContainer;
		ComponentEvents* ComponentEvents;
	};

	void MeshDrawSystem_init(MeshDrawSystem* p_meshDrawSystem, MeshDrawSystemInitInfo* p_meshDrawSystemInitInfo);
	void MeshDrawSystem_free(MeshDrawSystem* p_meshDrawSystem, ComponentEvents* p_componentEvents);
}