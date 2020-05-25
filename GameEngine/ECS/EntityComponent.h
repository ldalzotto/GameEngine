#pragma once

#include "Entity.h"
#include "Component.h"

namespace _GameEngine::_ECS
{
	struct EntityComponent
	{
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
	};

	EntityComponent* EntityComponent_alloc();
	void EntityComponent_free(EntityComponent** p_entityComponent);
};