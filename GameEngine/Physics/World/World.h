#pragma once

#include "DataStructures/VectorT.h"

namespace _GameEngine
{
	namespace _Physics
	{
		struct BoxCollider;
	}
	namespace _Log
	{
		struct MyLog;
	}
}

namespace _GameEngine::_Physics
{
	struct World 
	{
		_Log::MyLog* Log;
		_Core::VectorT<BoxCollider*> BoxColliders;
	};

	void World_alloc(World* p_world, _Log::MyLog* p_myLog);
	void World_free(World* p_world);

	 void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider);
	 void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider);
}