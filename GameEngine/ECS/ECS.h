#pragma once

#include "Entity.h"
#include "Component.h"

namespace _GameEngine::_ECS
{
	struct ECS
	{
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
	};

	ECS* EntityComponent_alloc();
	void EntityComponent_free(ECS** p_entityComponent);
};