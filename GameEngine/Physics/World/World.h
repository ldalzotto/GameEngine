#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Log/Log_def.h"

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
		Log_PTR Log;
		_Core::VectorT<BoxCollider*> BoxColliders;
	};

	void World_alloc(World* p_world, Log_PTR p_myLog);
	void World_free(World* p_world);

	 void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider);
	 void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider);
}