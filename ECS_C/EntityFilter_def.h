#pragma once

#include "Component_def.h"
#include "Entity_def.h"
#include "DataStructures/ARRAY_def.h"
#include "DataStructures/HASHMAP_def.h"

typedef struct ECS_EntityFilter_TYP
{
	Array_ComponentType FilteredComponentTypes;
	Array_ECSEntityHANDLE JustMatchedEntities;
	Array_ECSEntityHANDLE JustUnMatchedEntities;
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
