#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace _GameEngine
{
	struct UpdateSequencer;
}

namespace _GameEngine::_ECS
{
	struct ECS
	{
		UpdateSequencer* UpdateSequencer;
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
		SystemContainer SystemContainer;
	};

	ECS* EntityComponent_alloc(UpdateSequencer* p_updateSequencer);
	void EntityComponent_free(ECS** p_entityComponent);
};