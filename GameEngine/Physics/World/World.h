#pragma once

#include "DataStructures/VectorT.h"

namespace _Core
{
	struct Log;
}

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
		::_Core::Log* Log;
		_Core::VectorT<BoxCollider*> BoxColliders;
	};

	void World_alloc(World* p_world, ::_Core::Log* p_myLog);
	void World_free(World* p_world);

	 void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider);
	 void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider);
}