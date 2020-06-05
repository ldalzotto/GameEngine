#pragma once

#include "ECS/ECS.h"

namespace _GameEngine::_Render
{
	struct Render;
	struct DefaultMaterialDrawStep;
}

namespace _GameEngine::_ECS
{

	struct  MeshDrawSystem
	{
		ECS* ECS;
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	System* MeshDrawSystem_alloc(ECS* p_ecs);
}