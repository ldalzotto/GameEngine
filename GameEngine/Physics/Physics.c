#include "Physics.h"


#include "World/World.h"

void Physics_alloc(Physics* p_physics, Log* p_myLog)
{
	PhysicsInterface_build(&p_physics->PhysicsInterface, p_physics);
	World_alloc(&p_physics->World, p_myLog);
};

void Physics_free(Physics* p_physics)
{
	World_free(&p_physics->World);
};
