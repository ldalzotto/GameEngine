#pragma once

#include "HASHMAP_def.h"

void HashMap_Alloc(HashMap_PTR p_hashMap, const  HashMapEntryLayout_PTR p_entryLayout, HashMap_HashFn p_hashFunc, size_t p_initalCapacity);
void HashMap_Free(HashMap_PTR p_hashMap);

char HashMap_PushKeyValueRealloc(HashMap_PTR p_hashMap, const HashMapEntryLayout_PTR p_entryLayout, char* p_key, char* p_value);

char HashMap_GetValue(HashMap_PTR p_hashMap, const  HashMapEntryLayout_PTR p_entryLayout, char* p_key, char** out_value);