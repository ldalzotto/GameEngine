#pragma once

#include "PhysicsSystem_def.h"
#include "DataStructures/ARRAY.h"
#include "Physics/PhysicsInterface.h"

ARRAY_ALLOC_FUNCTION(PhysicsSystemOperation, Array_PhysicsSystemOperation_PTR, PhysicsSystemOperation)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(PhysicsSystemOperation, Array_PhysicsSystemOperation_PTR, PhysicsSystemOperation)
ARRAY_ERASE_FUNCTION(PhysicsSystemOperation, Array_PhysicsSystemOperation_PTR, PhysicsSystemOperation)

void PhysicsSystem_Alloc(PhysicsSystem_PTR p_physicsSystem, ECS* p_ecs);
void PhysicsSystem_Free(PhysicsSystem_PTR p_physicsSystem, ECS* p_ecs, PhysicsInterface_PTR p_physicsInterface);
void PhysicsSystem_Update(PhysicsSystem_PTR p_physicsSystem, PhysicsInterface_PTR p_physicsInterface);