#pragma once

#include "EntityFilter_def.h"
#include "DataStructures/ARRAY.h"
#include "DataStructures/HASHMAP.h"

#define myoffsetof(st, m) \
    ((size_t)((char *)&((st *)0)->m - (char *)0))

void ECS_EntityFilter_Alloc_1c(ECS_EntityFilter_PTR p_entityFilter, ECS_ComponentType p_filteredComponent1);
void ECS_EntityFilter_Alloc_2c(ECS_EntityFilter_PTR p_entityFilter, ECS_ComponentType p_filteredComponent1, ECS_ComponentType p_filteredComponent2);
void ECS_EntityFilter_Free(ECS_EntityFilter_PTR p_entityFilter);

ARRAY_ALLOC_FUNCTION(EntityFilterEvent, Array_EntityFilterEvent_PTR, ECS_EntityFilterEvent)
ARRAY_FREE_FUNCTION(EntityFilterEvent, Array_EntityFilterEvent_PTR)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(EntityFilterEvent, Array_EntityFilterEvent_PTR, ECS_EntityFilterEvent)

ARRAY_ALLOC_FUNCTION(EntityFilterPtr, Array_EntityFilterPtr_PTR, ECS_EntityFilter_PTR)
ARRAY_FREE_FUNCTION(EntityFilterPtr, Array_EntityFilterPtr_PTR)
ARRAY_PUSHBACKREALLOC_FUNCTION_VALUE(EntityFilterPtr, Array_EntityFilterPtr_PTR, ECS_EntityFilter_PTR)
ARRAY_ERASE_FUNCTION(EntityFilterPtr, Array_EntityFilterPtr_PTR, ECS_EntityFilter_PTR)

extern const HashMapEntryLayout HashMapEntry_EntityFilterByFilteredComponentType_Layout;
extern const HashMapEntryLayout_PTR HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR;

HASHMAP_ALLOC_FUNCTION(EntityFilterEvents, HashMap_EntityFilterEvents_PTR, ECS_ComponentType*, HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR);
HASHMAP_FREE_FUNCTION(EntityFilterEvents, HashMap_EntityFilterEvents_PTR);
HASHMAP_PUSH_KEYVALUE_REALLOC_FUNCTION(EntityFilterEvents, HashMap_EntityFilterEvents_PTR, ECS_ComponentType*, Array_EntityFilterPtr_PTR, HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR);
HASHMAP_GETVALUE_FUNCTION(EntityFilterEvents, HashMap_EntityFilterEvents_PTR, ECS_ComponentType*, Array_EntityFilterPtr_PTR, HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR);

void ECS_EntityFilter_Register(ECS* p_ecs, ECS_EntityFilter_PTR p_entityFilter);
void ECS_EntityFilter_UnRegister(ECS* p_ecs, ECS_EntityFilter_PTR p_entityFilter);