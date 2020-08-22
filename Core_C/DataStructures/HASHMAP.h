#pragma once

#include "HASHMAP_def.h"

void HashMap_Alloc(HashMap_PTR p_hashMap, size_t p_keySize, size_t p_valueSize, HashMap_HashFn p_hashFunc);
void HashMap_Free(HashMap_PTR p_hashMap);

char HashMap_PushKeyValueRealloc(HashMap_PTR p_hashMap, size_t p_keySize, size_t p_valueSize, char* p_key, char* p_value);
char HashMap_GetValue(HashMap_PTR p_hashMap, size_t p_keySize, size_t p_valueSize, char* p_key, char** out_value);