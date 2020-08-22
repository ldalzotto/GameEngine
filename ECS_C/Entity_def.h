#pragma once

#include "Component_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct ECS_Entity_TYP
{
	Array_ComponentHeaderHandle Components;
}ECS_Entity, * ECS_Entity_HANDLE;

typedef struct Array_ECSEntityHANDLE_TYP
{
	ARRAY_TYPE_DEFINITION(ECS_Entity_HANDLE)
}Array_ECSEntityHANDLE, * Array_ECSEntityHANDLE_PTR;

typedef Array_ECSEntityHANDLE ECS_EntityContainer, * ECS_EntityContainer_PTR;