#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "ECSEventQueue.h"
extern "C"
{
#include "Log/Log_def.h"
}

namespace _GameEngine::_ECS
{
	struct ECS
	{
		Log_PTR MyLog;

		ECSEventQueue EventQueue;
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
		SystemContainerV2 SystemContainerV2;
		SystemEvents SystemEvents;
	};

	void EntityComponent_build(ECS* p_ecs, Log_PTR p_myLog);
	void EntityComponent_free(ECS* p_entityComponent);
};