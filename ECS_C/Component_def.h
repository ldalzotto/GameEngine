#pragma once

#include "DataStructures/ARRAY_def.h"

typedef struct ECS_Entity_TYP ECS_Entity, * ECS_Entity_HANDLE;

typedef size_t ECS_ComponentType;

typedef struct ECS_ComponentHeader_TYP
{
	ECS_Entity_HANDLE AttachedEntity;
	ECS_ComponentType ComponentType;
}ECS_ComponentHeader, * ECS_ComponentHeader_HANDLE;

typedef struct Array_ComponentHeaderHandle_TYP
{
	ARRAY_TYPE_DEFINITION(ECS_ComponentHeader_HANDLE)
}Array_ComponentHeaderHandle, * Array_ComponentHeaderHandle_PTR;

typedef void(*ECS_OnComponentDestroyedFn)(ECS_ComponentHeader_HANDLE, void* p_closure);

typedef struct ECS_OnComponentDestroyedStaticCallback_TYP
{
	ECS_ComponentType ComponentType;
	ECS_OnComponentDestroyedFn Function;
	void* Closure;
}ECS_OnComponentDestroyedStaticCallback, * ECS_OnComponentDestroyedStaticCallback_PTR;