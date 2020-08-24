#pragma once

#include "World_def.h"

void World_alloc(World* p_world, Log_PTR p_myLog);
void World_free(World* p_world);

void World_pushBoxCollider(World* p_world, BoxCollider* p_boxCollider);
void World_removeBoxCollider(World* p_world, BoxCollider* p_boxCollider);
