#pragma once

#include "EntityFilter_def.h"
#include "DataStructures/ARRAY.h"
#include "DataStructures/HASHMAP.h"

void ECS_EntityFilter_Alloc_1c(ECS_EntityFilter_PTR p_entityFilter, ECS_ComponentType p_filteredComponent1);
void ECS_EntityFilter_Alloc_2c(ECS_EntityFilter_PTR p_entityFilter, ECS_ComponentType p_filteredComponent1, ECS_ComponentType p_filteredComponent2);
void ECS_EntityFilter_Free(ECS_EntityFilter_PTR p_entityFilter);

inline void Arr_Alloc_EntityFilterPtr(Array_EntityFilterPtr_PTR p_array, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(ECS_EntityFilter_PTR), p_initialCapacity); };
inline void Arr_Free_EntityFilterPtr(Array_EntityFilterPtr_PTR p_array) { Arr_Free((Array_PTR)p_array); };
inline void Arr_PushbackRealloc_EntityFilterPtr(Array_EntityFilterPtr_PTR p_array, ECS_EntityFilter_PTR p_entityFilter) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(ECS_EntityFilter_PTR), (char*)&p_entityFilter); }
inline void Arr_Erase_EntityFilterPtr(Array_EntityFilterPtr_PTR p_array, size_t p_index) { Arr_Erase((Array_PTR)p_array, sizeof(ECS_EntityFilter_PTR), p_index); };

inline void HashMap_Alloc_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap, size_t(*HashFn)(ECS_ComponentType*), size_t p_initialCapacity) { HashMap_Alloc((HashMap_PTR)p_hashMap, sizeof(ECS_ComponentType), sizeof(Array_EntityFilterPtr), (HashMap_HashFn)HashFn, p_initialCapacity); }
inline void HashMap_Free_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap) { HashMap_Free((HashMap_PTR)p_hashMap); }
inline char HashMap_PushKeyValueRealloc_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap, ECS_ComponentType p_key, Array_EntityFilterPtr_PTR p_value) { return HashMap_PushKeyValueRealloc((HashMap_PTR)p_hashMap, sizeof(ECS_ComponentType), sizeof(Array_EntityFilterPtr), (char*)&p_key, (char*)p_value); };
inline char Hashmap_GetValue_EntityFilterEvents(HashMap_EntityFilterEvents_PTR p_hashMap, ECS_ComponentType p_key, Array_EntityFilterPtr_PTR* out_value) { return HashMap_GetValue((HashMap_PTR)p_hashMap, sizeof(ECS_ComponentType), sizeof(Array_EntityFilterPtr), (char*)&p_key, (char**)out_value); }

void ECS_EntityFilter_Register(ECS* p_ecs, ECS_EntityFilter_PTR p_entityFilter);
void ECS_EntityFilter_UnRegister(ECS* p_ecs, ECS_EntityFilter_PTR p_entityFilter);