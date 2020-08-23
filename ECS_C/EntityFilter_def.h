#pragma once

#include "Component_def.h"
#include "Entity_def.h"
#include "DataStructures/ARRAY_def.h"
#include "DataStructures/HASHMAP_def.h"

typedef char ECS_EntityFilterEventType;
#define EntityFilterEventType_ConditionsJustMet 0
#define EntityFilterEventType_ConditionsJustNotMet 1

typedef struct ECS_EntityFilterEvent_TYP
{
	ECS_EntityFilterEventType Type;
	ECS_Entity_HANDLE Entity;
}ECS_EntityFilterEvent, * ECS_EntityFilterEvent_PTR;

typedef struct Array_EntityFilterEvent_TYP
{
	ARRAY_TYPE_DEFINITION(ECS_EntityFilterEvent)
}Array_EntityFilterEvent, * Array_EntityFilterEvent_PTR;

typedef struct ECS_EntityFilter_TYP
{
	Array_ComponentType FilteredComponentTypes;
	Array_EntityFilterEvent EntityFilterEvents;
}ECS_EntityFilter, * ECS_EntityFilter_PTR;

typedef struct Array_EntityFilterPtr_TYP
{
	ARRAY_TYPE_DEFINITION(ECS_EntityFilter_PTR)
}Array_EntityFilterPtr, * Array_EntityFilterPtr_PTR;

typedef struct HashMapEntry_EntityFilterByFilteredComponentType_TYP
{
	HASHMAPENTRY_TYPE_DEFIINITION(ECS_ComponentType, Array_EntityFilterPtr)
}HashMapEntry_EntityFilterByFilteredComponentType, *HashMapEntry_EntityFilterByFilteredComponentType_PTR;

typedef struct HashMap_EntityFilterEvents_TYP
{
	HASHMAP_TYPE_DEFINITION(HashMapEntry_EntityFilterByFilteredComponentType)
}HashMap_EntityFilterEvents, * HashMap_EntityFilterEvents_PTR;
