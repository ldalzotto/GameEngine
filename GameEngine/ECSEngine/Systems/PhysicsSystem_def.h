#pragma once

#include "EntityFilter_def.h"
#include "ECS_def.h"
#include "World/Collider/BoxCollider_def.h"

typedef struct PhysicsSystemOperation_TYP
{
	ECS_Entity_HANDLE Entity;
	BoxCollider_PTR BoxCollider;
}PhysicsSystemOperation, * PhysicsSystemOperation_PTR;

typedef struct Array_PhysicsSystemOperation_TYP
{
	ARRAY_TYPE_DEFINITION(PhysicsSystemOperation)
}Array_PhysicsSystemOperation, * Array_PhysicsSystemOperation_PTR;

typedef struct PhysicsSystem_TYP
{
	ECS_EntityFilter PhysicsAwareFilter;
	Array_PhysicsSystemOperation PhysicsOperations;
}PhysicsSystem, * PhysicsSystem_PTR;