#pragma once

#include "HASHMAP_def.h"


void HashMap_Alloc(HashMap_PTR p_hashMap, const  HashMapEntryLayout_PTR p_entryLayout, HashMap_HashFn p_hashFunc, size_t p_initalCapacity);
void HashMap_Free(HashMap_PTR p_hashMap);

char HashMap_PushKeyValueRealloc(HashMap_PTR p_hashMap, const HashMapEntryLayout_PTR p_entryLayout, char* p_key, char* p_value);

char HashMap_GetValue(HashMap_PTR p_hashMap, const  HashMapEntryLayout_PTR p_entryLayout, char* p_key, char** out_value);


#define HASHMAP_ALLOC_FUNCTION(MethodPrefix, HashMapPTRType, KeyTypePTR, EntryLayoutConstantPTR) inline void HashMap_Alloc_##MethodPrefix##(HashMapPTRType p_hashMap, size_t(*HashFn)(KeyTypePTR), size_t p_initialCapacity) { HashMap_Alloc((HashMap_PTR)p_hashMap, EntryLayoutConstantPTR, (HashMap_HashFn)HashFn, p_initialCapacity); };
#define HASHMAP_FREE_FUNCTION(MethodPrefix, HashMapPTRType) inline void HashMap_Free_##MethodPrefix##(HashMapPTRType p_hashMap) { HashMap_Free((HashMap_PTR)p_hashMap); };
#define HASHMAP_PUSH_KEYVALUE_REALLOC_FUNCTION(MethodPrefix, HashMapPTRType, KeyTypePTR, ValueTypePTR, EntryLayoutConstantPTR) inline char HashMap_PushKeyValueRealloc_##MethodPrefix##(HashMapPTRType p_hashMap, KeyTypePTR p_key, ValueTypePTR p_value) { return HashMap_PushKeyValueRealloc((HashMap_PTR)p_hashMap, EntryLayoutConstantPTR, (char*)p_key, (char*)p_value); };
#define HASHMAP_GETVALUE_FUNCTION(MethodPrefix, HashMapPTRType, KeyTypePTR, ValueTypePTR, EntryLayoutConstantPTR) inline char Hashmap_GetValue_##MethodPrefix##(HashMapPTRType p_hashMap, KeyTypePTR p_key, ValueTypePTR* out_value) { return HashMap_GetValue((HashMap_PTR)p_hashMap, EntryLayoutConstantPTR, (char*)p_key, (char**)out_value); };