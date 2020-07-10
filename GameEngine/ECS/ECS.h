#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "ECSEventQueue.h"

struct Core_Log;

namespace _GameEngine::_ECS
{
	struct ECS
	{
		Core_Log* MyLog;

		ECSEventQueue EventQueue;
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
		SystemContainer SystemContainer;
	};

	void EntityComponent_build(ECS* p_ecs, Core_Log* p_myLog);
	void EntityComponent_free(ECS* p_entityComponent);
};