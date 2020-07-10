#pragma once

#include "PhysicsInterface.h"
#include "World/World.h"

struct Core_Log;

namespace _GameEngine::_Physics
{
	struct Physics
	{
		PhysicsInterface PhysicsInterface;
		World World;
	};

	void Physics_alloc(Physics* p_physics, Core_Log* p_myLog);
	void Physics_free(Physics* p_physics);
}