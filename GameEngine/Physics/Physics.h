#pragma once

#include "PhysicsInterface.h"
#include "World/World.h"

typedef struct Log_TYP Log;

namespace _GameEngine::_Physics
{
	struct Physics
	{
		PhysicsInterface PhysicsInterface;
		World World;
	};

	void Physics_alloc(Physics* p_physics, Log* p_myLog);
	void Physics_free(Physics* p_physics);
}