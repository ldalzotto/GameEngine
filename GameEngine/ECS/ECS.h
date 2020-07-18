#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "ECSEventQueue.h"

namespace _Core
{
	struct Log;
}

namespace _GameEngine::_ECS
{
	struct ECS
	{
		::_Core::Log* MyLog;

		ECSEventQueue EventQueue;
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
		SystemContainer SystemContainer;
		SystemContainerV2 SystemContainerV2;
	};

	void EntityComponent_build(ECS* p_ecs, ::_Core::Log* p_myLog);
	void EntityComponent_free(ECS* p_entityComponent);
};