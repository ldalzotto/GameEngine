#pragma once

#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace _GameEngine::_Utils
{
	struct SortedSequencer;
}

namespace _GameEngine::_ECS
{
	struct ECS
	{
		_Utils::SortedSequencer* UpdateSortedSequencer;
		EntityContainer EntityContainer;
		ComponentEvents ComponentEvents;
		SystemContainer SystemContainer;
	};

	ECS* EntityComponent_alloc();
	void EntityComponent_free(ECS** p_entityComponent);
};