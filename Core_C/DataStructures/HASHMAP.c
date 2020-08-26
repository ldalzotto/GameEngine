#include "HASHMAP.h"

#include "Error/ErrorHandler.h"

#include <string.h>
#include <stdlib.h>

inline void memcpy_hashmapsafe_dist(char* p_dist, const char* p_source, const size_t p_size, const HashMap_PTR p_distMap, const size_t p_distMapEntrySize)
{
#if BUFFER_OVERFLOW_INTERNAL_CONDITION
	HandleBufferOverflowMemoryWrite(p_dist, p_size, p_distMap->Entries, p_distMap->Capacity * p_distMapEntrySize);
#endif
	memcpy(p_dist, p_source, p_size);
};


inline char* HashMapEntry_GetKey(HashMapEntryHeader_PTR p_header, const  HashMapEntryLayout_PTR p_entryLayout)
{
	return ((char*)&p_header->IsOccupied) + p_entryLayout->KeyOffset;
};

inline char* HashMapEntry_GetValue(HashMapEntryHeader_PTR p_header, const HashMapEntryLayout_PTR p_entryLayout)
{
	return ((char*)&p_header->IsOccupied) + p_entryLayout->ValueOffset;
};

void HashMap_Alloc(HashMap_PTR p_hashMap, const HashMapEntryLayout_PTR p_entryLayout, HashMap_HashFn p_hashFunc, size_t p_initalCapacity)
{
	p_hashMap->Entries = calloc(1, p_initalCapacity * p_entryLayout->TotalSize);
	p_hashMap->Capacity = p_initalCapacity;
	p_hashMap->HashFn = p_hashFunc;
};

void HashMap_Free(HashMap_PTR p_hashMap)
{
	free(p_hashMap->Entries);
	*p_hashMap = (HashMap) { 0 };
};

inline size_t HashMap_CalculateKeyIndex_FromKey(HashMap_PTR p_hashMap, char* p_key, size_t p_maxValueExcluded)
{
	return p_hashMap->HashFn(p_key) % p_maxValueExcluded;
};

inline size_t HashMap_CalculateKeyIndex_FromHash(size_t p_hash, size_t p_maxValueExcluded)
{
	return p_hash % p_maxValueExcluded;
};


inline char* HashMap_GetEntryMemory(char* p_entries, const  HashMapEntryLayout_PTR p_entryLayout, size_t p_index)
{
	return p_entries + (p_entryLayout->TotalSize * p_index);
}

void HashMap_ReallocateEntries(HashMap_PTR p_hashMap, const  HashMapEntryLayout_PTR p_entryLayout, size_t p_newCapacity)
{
	if (p_newCapacity > p_hashMap->Capacity)
	{
		char* l_newEntries;
		l_newEntries = (char*) calloc(1, p_entryLayout->TotalSize * p_newCapacity);

		HashMapEntryHeader* l_hashMapEntryCursor = (HashMapEntryHeader*)(p_hashMap->Entries - p_entryLayout->TotalSize);
		for (size_t i = 0; i < p_hashMap->Capacity; i++)
		{
			l_hashMapEntryCursor = (HashMapEntryHeader*)((char*)l_hashMapEntryCursor + p_entryLayout->TotalSize);
			if (l_hashMapEntryCursor->IsOccupied)
			{
				char* l_key = HashMapEntry_GetKey(l_hashMapEntryCursor, p_entryLayout);
				size_t l_index = HashMap_CalculateKeyIndex_FromKey(p_hashMap, l_key, p_newCapacity);
				// memcpy(HashMap_GetEntryMemory(l_newEntries, p_entryLayout, l_index), (char*)l_hashMapEntryCursor, p_entryLayout->TotalSize);
				memcpy_hashmapsafe_dist(HashMap_GetEntryMemory(l_newEntries, p_entryLayout, l_index), (char*)l_hashMapEntryCursor, p_entryLayout->TotalSize, p_hashMap, p_entryLayout->TotalSize);
			}
		}

		free(p_hashMap->Entries);
		p_hashMap->Entries = l_newEntries;
		p_hashMap->Capacity = p_newCapacity;
	}
};

char HashMap_PushKeyValueRealloc(HashMap_PTR p_hashMap, const  HashMapEntryLayout_PTR p_entryLayout, char* p_key, char* p_value)
{
	if (p_hashMap->Capacity == 0)
	{
		HashMap_ReallocateEntries(p_hashMap, p_entryLayout, p_hashMap->Capacity == 0 ? 1 : (p_hashMap->Capacity * 2));
		HashMap_PushKeyValueRealloc(p_hashMap, p_entryLayout, p_key, p_value);
	}
	else
	{
		size_t l_keyHash = p_hashMap->HashFn(p_key);
		size_t l_keyIndex = HashMap_CalculateKeyIndex_FromHash(l_keyHash, p_hashMap->Capacity);

		HashMapEntryHeader* l_hashMapEntryHeader = (HashMapEntryHeader*)HashMap_GetEntryMemory(p_hashMap->Entries, p_entryLayout, l_keyIndex);
		if (l_hashMapEntryHeader->IsOccupied)
		{
			char* l_existingKey = HashMapEntry_GetKey(l_hashMapEntryHeader, p_entryLayout);
			if (p_hashMap->HashFn(l_existingKey) == l_keyHash)
			{
				//TODO -> ERROR we are trying to insert the same key
				return 0;
			}
			else
			{
				HashMap_ReallocateEntries(p_hashMap, p_entryLayout, p_hashMap->Capacity == 0 ? 1 : (p_hashMap->Capacity * 2));
				return HashMap_PushKeyValueRealloc(p_hashMap, p_entryLayout, p_key, p_value);
			}
		}
		else
		{
			char* l_hashMapEntryHeaderCursor = (char*)l_hashMapEntryHeader;
			*l_hashMapEntryHeaderCursor = 1;
			l_hashMapEntryHeaderCursor  = (char*)l_hashMapEntryHeader + p_entryLayout->KeyOffset;
			memcpy_hashmapsafe_dist(l_hashMapEntryHeaderCursor, p_key, p_entryLayout->KeySize, p_hashMap, p_entryLayout->TotalSize);
			l_hashMapEntryHeaderCursor = (char*)l_hashMapEntryHeader + p_entryLayout->ValueOffset;
			memcpy_hashmapsafe_dist(l_hashMapEntryHeaderCursor, p_value, p_entryLayout->ValueSize, p_hashMap, p_entryLayout->TotalSize);
		}
	}

	return 1;
};

char HashMap_GetValue(HashMap_PTR p_hashMap, const  HashMapEntryLayout_PTR p_entryLayout, char* p_key, char** out_value)
{
	size_t l_keyHash = p_hashMap->HashFn(p_key);
	size_t l_keyIndex = HashMap_CalculateKeyIndex_FromHash(l_keyHash, p_hashMap->Capacity);
	HashMapEntryHeader* l_hashMapEntryHeader = (HashMapEntryHeader*)HashMap_GetEntryMemory(p_hashMap->Entries, p_entryLayout, l_keyIndex);
	if (l_hashMapEntryHeader->IsOccupied)
	{
		char* l_existingKey = HashMapEntry_GetKey(l_hashMapEntryHeader, p_entryLayout);
		if (p_hashMap->HashFn(l_existingKey) == l_keyHash)
		{
			*out_value = HashMapEntry_GetValue(l_hashMapEntryHeader, p_entryLayout);
			return 1;
		}

	}

	return 0;
};