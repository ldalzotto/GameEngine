#pragma once

#include "PhysicsInterface.h"
#include "World/World.h"

namespace _GameEngine::_Physics
{
	struct Physics
	{
		PhysicsInterface PhysicsInterface;
		World World;
	};

	void Physics_alloc(Physics* p_physics);
	void Physics_free(Physics* p_physics);
}