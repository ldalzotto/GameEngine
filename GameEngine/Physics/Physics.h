#pragma once

#include "PhysicsInterface.h"
#include "World/World_def.h"

typedef struct Log_TYP Log;

typedef	struct Physics_TYP
{
	PhysicsInterface PhysicsInterface;
	World World;
}Physics, * Physics_PTR;

void Physics_alloc(Physics* p_physics, Log* p_myLog);
void Physics_free(Physics* p_physics);
