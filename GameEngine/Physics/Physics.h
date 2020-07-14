#pragma once

#include "PhysicsInterface.h"
#include "World/World.h"

namespace _Core
{
struct Log;
}

namespace _GameEngine::_Physics
{
	struct Physics
	{
		PhysicsInterface PhysicsInterface;
		World World;
	};

	void Physics_alloc(Physics* p_physics, ::_Core::Log* p_myLog);
	void Physics_free(Physics* p_physics);
}