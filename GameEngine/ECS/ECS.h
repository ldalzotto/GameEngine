#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace _GameEngine
{
	struct UpdateSequencer;
	namespace _Log
	{
		struct MyLog;
	}
}

namespace _GameEngine::_ECS
{
	struct ECS
	{
		UpdateSequencer* UpdateSequencer;
		_Log::MyLog* MyLog;
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
		SystemContainer SystemContainer;
	};

	void EntityComponent_build(ECS* p_ecs, UpdateSequencer* p_updateSequencer, _Log::MyLog* p_myLog);
	void EntityComponent_free(ECS* p_entityComponent);
};