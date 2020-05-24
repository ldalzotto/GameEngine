#pragma once

#include "Entity.h"

namespace _GameEngine::_ECS
{
	struct EntityComponent
	{
		EntityContainer EntityContainer;
	};

	EntityComponent* EntityComponent_alloc();
	void EntityComponent_free(EntityComponent** p_entityComponent);
};