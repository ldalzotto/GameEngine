#include "World.h"

#include "Log/Log.hpp"

#include "Collider/BoxCollider.h"

namespace _GameEngine::_Physics
{
	void World_alloc(World* p_world, ::_Core::Log* p_myLog)
	{
		p_world->Log = p_myLog;
		p_world->BoxColliders.alloc();
	};
	
	void World_free(World* p_world)
	{

#ifndef NDEBUG
		if (p_world->BoxColliders.size() > 0)
		{
			MYLOG_PUSH(p_world->Log, ::_Core::LogLevel::WARN, "Potential memory leak. When freeing the physics world, there was still BoxColliders. Consider cleaning them before freeing the World.");
		}
#endif

		p_world->BoxColliders.free();
	};


	void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider)
	{
		p_world->BoxColliders.push_back(&p_boxCollider);
	};

	void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider)
	{
		p_world->BoxColliders.erase(BoxCollider_equals, &p_boxCollider);
	};

}