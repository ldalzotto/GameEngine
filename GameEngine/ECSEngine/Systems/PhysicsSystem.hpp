#pragma once

extern "C"
{
//#include "Entity_def.h"
#include "EntityFilter_def.h"
#include "ECS_def.h"
#include "DataStructures/ARRAY.h"
}

namespace _GameEngine::_Physics
{
	struct PhysicsInterface;
	struct BoxCollider;
};

typedef struct PhysicsSystemOperation_TYP
{
	ECS_Entity_HANDLE Entity;
	_GameEngine::_Physics::BoxCollider* BoxCollider;
}PhysicsSystemOperation, * PhysicsSystemOperation_PTR;

typedef struct Array_PhysicsSystemOperation_TYP
{
	ARRAY_TYPE_DEFINITION(PhysicsSystemOperation)
}Array_PhysicsSystemOperation, * Array_PhysicsSystemOperation_PTR;

ARRAY_ALLOC_FUNCTION(PhysicsSystemOperation, Array_PhysicsSystemOperation_PTR, PhysicsSystemOperation)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(PhysicsSystemOperation, Array_PhysicsSystemOperation_PTR, PhysicsSystemOperation)
ARRAY_ERASE_FUNCTION(PhysicsSystemOperation, Array_PhysicsSystemOperation_PTR, PhysicsSystemOperation)

typedef struct PhysicsSystem_TYP
{
	ECS_EntityFilter PhysicsAwareFilter;
	Array_PhysicsSystemOperation PhysicsOperations;
}PhysicsSystem, * PhysicsSystem_PTR;

void PhysicsSystem_Alloc(PhysicsSystem_PTR p_physicsSystem, ECS* p_ecs);
void PhysicsSystem_Free(PhysicsSystem_PTR p_physicsSystem, ECS* p_ecs, _GameEngine::_Physics::PhysicsInterface* p_physicsInterface);
void PhysicsSystem_Update(PhysicsSystem_PTR p_physicsSystem, _GameEngine::_Physics::PhysicsInterface* p_physicsInterface);