#pragma once

#include "PhysicsInterface.h"
#include "World/World.h"

namespace _GameEngine::_Log
{
	struct MyLog;
}

namespace _GameEngine::_Physics
{
	struct Physics
	{
		PhysicsInterface PhysicsInterface;
		World World;
	};

	void Physics_alloc(Physics* p_physics, _Log::MyLog* p_myLog);
	void Physics_free(Physics* p_physics);
}