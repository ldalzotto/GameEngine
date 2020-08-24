#include "World.h"


#include "Log/Log.h"
#include "Collider/BoxCollider.h"



void World_alloc(World* p_world, Log_PTR p_myLog)
{
	p_world->Log = p_myLog;
	Arr_Alloc_BoxColliderPTR(&p_world->BoxColliders, 0);
};

void World_free(World* p_world)
{

#ifndef NDEBUG
	if (p_world->BoxColliders.Size > 0)
	{
		MYLOG_PUSH(p_world->Log, LogLevel_WARN, "Potential memory leak. When freeing the physics world, there was still BoxColliders. Consider cleaning them before freeing the World.");
	}
#endif
	Arr_Free_BoxColliderPTR(&p_world->BoxColliders);
};


void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider)
{
	Arr_PushBackRealloc_BoxColliderPTR(&p_world->BoxColliders, p_boxCollider);
};

void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider)
{
	for (size_t i = 0; i < p_world->BoxColliders.Size; i++)
	{
		if (p_world->BoxColliders.Memory[i] == p_boxCollider)
		{
			Arr_Erase_BoxColliderPTR(&p_world->BoxColliders, i);
		}
	}
};
