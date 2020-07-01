#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "ECSEventQueue.h"

namespace _GameEngine
{
	namespace _Log
	{
		struct MyLog;
	}
}

namespace _GameEngine::_ECS
{
	struct ECS
	{
		_Log::MyLog* MyLog;

		ECSEventQueue EventQueue;
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
		SystemContainer SystemContainer;
	};

	void EntityComponent_build(ECS* p_ecs, _Log::MyLog* p_myLog);
	void EntityComponent_free(ECS* p_entityComponent);
};