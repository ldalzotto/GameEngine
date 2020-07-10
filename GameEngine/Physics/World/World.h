#pragma once

#include "DataStructures/VectorT.h"

struct Core_Log;

namespace _GameEngine
{
	namespace _Physics
	{
		struct BoxCollider;
	}
}

namespace _GameEngine::_Physics
{
	struct World 
	{
		struct Core_Log* Log;
		_Core::VectorT<BoxCollider*> BoxColliders;
	};

	void World_alloc(World* p_world, Core_Log* p_myLog);
	void World_free(World* p_world);

	 void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider);
	 void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider);
}