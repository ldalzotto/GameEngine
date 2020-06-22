#pragma once

#include "DataStructures/VectorT.h"

namespace _GameEngine::_Physics
{
	struct BoxCollider;
}

namespace _GameEngine::_Physics
{
	struct World 
	{
		_Core::VectorT<BoxCollider*> BoxColliders;
	};

	void World_alloc(World* p_world);
	void World_free(World* p_world);

	// BoxCollider* World_allocBoxCollider(World* p_world);
}