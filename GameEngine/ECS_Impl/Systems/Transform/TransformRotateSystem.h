
#include "ECS/Entity.h"

namespace _GameEngine::_ECS
{
	struct ECS;
	struct System;
}

namespace _GameEngine::_ECS
{
	struct TransformRotateSystem
	{
		ECS* ECS;
		EntityConfigurableContainer EntityConfigurableContainer;
	};

	System* TransformRotateSystem_alloc(ECS* p_ecs);
}