#include "World.h"

#include "Collider/BoxCollider.h"

namespace _GameEngine::_Physics
{
	void World_alloc(World* p_world)
	{
		p_world->BoxColliders.alloc();
	};
	
	void World_free(World* p_world)
	{
		p_world->BoxColliders.free();
	};

	/*
	BoxCollider* World_allocBoxCollider(World* p_world)
	{
		BoxCollider* l_boxCollider = new BoxCollider();
		p_world->BoxColliders.push_back(&l_boxCollider);
		return l_boxCollider;
	};
	*/
}