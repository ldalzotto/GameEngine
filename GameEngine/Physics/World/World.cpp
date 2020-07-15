#include "World.h"

#include "Log/Log.hpp"

#include "Collider/BoxCollider.h"

namespace _GameEngine::_Physics
{
	void World_alloc(World* p_world, ::_Core::Log* p_myLog)
	{
		p_world->Log = p_myLog;
		_Core::VectorT_alloc(&p_world->BoxColliders, 0);
	};
	
	void World_free(World* p_world)
	{

#ifndef NDEBUG
		if (p_world->BoxColliders.Size > 0)
		{
			MYLOG_PUSH(p_world->Log, ::_Core::LogLevel::WARN, "Potential memory leak. When freeing the physics world, there was still BoxColliders. Consider cleaning them before freeing the World.");
		}
#endif
		_Core::VectorT_free(&p_world->BoxColliders);
	};


	void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider)
	{
		_Core::VectorT_pushBack(&p_world->BoxColliders, &p_boxCollider);
	};

	void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider)
	{
		_Core::VectorT_eraseCompare(&p_world->BoxColliders, _Core::ComparatorT<BoxCollider*, BoxCollider*, void>{ BoxCollider_equals , &p_boxCollider });
	};

}