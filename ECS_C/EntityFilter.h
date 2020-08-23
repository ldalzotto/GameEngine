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

const HashMapEntryLayout HashMapEntry_EntityFilterByFilteredComponentType_Layout = {
	.KeyOffset = myoffsetof(HashMapEntry_EntityFilterByFilteredComponentType, Key),
	.ValueOffset = myoffsetof(HashMapEntry_EntityFilterByFilteredComponentType, Value),
	.TotalSize = sizeof(HashMapEntry_EntityFilterByFilteredComponentType),
	.KeySize = sizeof(ECS_ComponentType),
	.ValueSize = sizeof(Array_EntityFilterPtr)
};
const HashMapEntryLayout_PTR HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR = (const HashMapEntryLayout_PTR)&HashMapEntry_EntityFilterByFilteredComponentType_Layout;

inline void HashMap_Alloc_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap, size_t(*HashFn)(ECS_ComponentType*), size_t p_initialCapacity) { HashMap_Alloc((HashMap_PTR)p_hashMap, HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR, (HashMap_HashFn)HashFn, p_initialCapacity); }
inline void HashMap_Free_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap) { HashMap_Free((HashMap_PTR)p_hashMap); }
inline char HashMap_PushKeyValueRealloc_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap, ECS_ComponentType p_key, Array_EntityFilterPtr_PTR p_value) { return HashMap_PushKeyValueRealloc((HashMap_PTR)p_hashMap, HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR, (char*)&p_key, (char*)p_value); };
inline char Hashmap_GetValue_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap, ECS_ComponentType p_key, Array_EntityFilterPtr_PTR* out_value) { return HashMap_GetValue((HashMap_PTR)p_hashMap, HashMapEntry_EntityFilterByFilteredComponentType_Layout_PTR, (char*)&p_key, (char**)out_value); }

void ECS_EntityFilter_Register(ECS* p_ecs, ECS_EntityFilter_PTR p_entityFilter);
void ECS_EntityFilter_UnRegister(ECS* p_ecs, ECS_EntityFilter_PTR p_entityFilter);