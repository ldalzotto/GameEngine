#include "HASHMAP.h"

#include "ARRAY.h"

#include <string.h>

typedef struct HashMapEntryHeader_TYP
{
	char IsOccupied;
} HashMapEntryHeader, * HashMapEntryHeader_PTR;

inline size_t HashMapEntry_GetMemorySize(size_t p_keySize, size_t p_valueSize)
{
	return sizeof(char) + p_keySize + p_valueSize;
};

inline char* HashMapEntry_GetKey(HashMapEntryHeader_PTR p_header)
{
	return ((char*)&p_header->IsOccupied) + sizeof(char);
};

inline char* HashMapEntry_GetValue(HashMapEntryHeader_PTR p_header, size_t p_keySize)
{
	return ((char*)&p_header->IsOccupied) + sizeof(char) + p_keySize;
};

void HashMap_Alloc(HashMap_PTR p_hashMap, size_t p_keySize, size_t p_valueSize, HashMap_HashFn p_hashFunc)
{
	Arr_Alloc(&p_hashMap->Entries, HashMapEntry_GetMemorySize(p_keySize, p_valueSize), 0);
	Arr_Zeroing(&p_hashMap->Entries, HashMapEntry_GetMemorySize(p_keySize, p_valueSize));
	p_hashMap->HashFn = p_hashFunc;
};

void HashMap_Free(HashMap_PTR p_hashMap)
{
	Arr_Free(&p_hashMap->Entries);
};

inline size_t HashMap_CalculateKeyIndex(HashMap_PTR p_hashMap, char* p_key, size_t p_maxValueExcluded)
{
	return p_hashMap->HashFn(p_key) % p_maxValueExcluded;
}

inline char* HashMap_GetEntryMemory(Array_PTR p_entries, size_t p_keySize, size_t p_valueSize, size_t p_index)
{
	return p_entries->Memory + (HashMapEntry_GetMemorySize(p_keySize, p_valueSize) * p_index);
}

void HashMap_ReallocateEntries(HashMap_PTR p_hashMap, size_t p_keySize, size_t p_valueSize, size_t p_newCapacity)
{
	if (p_newCapacity > p_hashMap->Entries.Capacity)
	{
		Array l_newEntries;
		Arr_Alloc(&l_newEntries, HashMapEntry_GetMemorySize(p_keySize, p_valueSize), p_newCapacity);
		Arr_Zeroing(&l_newEntries, HashMapEntry_GetMemorySize(p_keySize, p_valueSize));

		for (size_t i = 0; i < p_hashMap->Entries.Capacity; i++)
		{
			HashMapEntryHeader* l_hashMapEntry = (HashMapEntryHeader*)HashMap_GetEntryMemory(&p_hashMap->Entries, p_keySize, p_valueSize, i);
			if (l_hashMapEntry->IsOccupied)
			{
				char* l_key = HashMapEntry_GetKey(l_hashMapEntry);
				size_t l_index = HashMap_CalculateKeyIndex(p_hashMap, l_key, l_newEntries.Capacity);
				memcpy(HashMap_GetEntryMemory(&l_newEntries, p_keySize, p_valueSize, l_index), (char*)l_hashMapEntry, HashMapEntry_GetMemorySize(p_keySize, p_valueSize));
			}
		}

		Arr_Free(&p_hashMap->Entries);
		p_hashMap->Entries = l_newEntries;
	}
};

char HashMap_PushKeyValueRealloc(HashMap_PTR p_hashMap, size_t p_keySize, size_t p_valueSize, char* p_key, char* p_value)
{
	if (p_hashMap->Entries.Capacity == 0)
	{
		HashMap_ReallocateEntries(p_hashMap, p_keySize, p_valueSize, p_hashMap->Entries.Capacity == 0 ? 1 : (p_hashMap->Entries.Capacity * 2));
		HashMap_PushKeyValueRealloc(p_hashMap, p_keySize, p_valueSize, p_key, p_value);
	}
	else
	{
		size_t l_keyIndex = HashMap_CalculateKeyIndex(p_hashMap, p_key, p_hashMap->Entries.Capacity);

		HashMapEntryHeader* l_hashMapEntryHeader = (HashMapEntryHeader*)HashMap_GetEntryMemory(&p_hashMap->Entries, p_keySize, p_valueSize, l_keyIndex);
		if (l_hashMapEntryHeader->IsOccupied)
		{
			char* l_existingKey = HashMapEntry_GetKey(l_hashMapEntryHeader);
			if (p_hashMap->HashFn(l_existingKey) == p_hashMap->HashFn(p_key))
			{
				//TODO -> ERROR we are trying to insert the same key
				return 0;
			}
			else
			{
				HashMap_ReallocateEntries(p_hashMap, p_keySize, p_valueSize, p_hashMap->Entries.Capacity == 0 ? 1 : (p_hashMap->Entries.Capacity * 2));
				return HashMap_PushKeyValueRealloc(p_hashMap, p_keySize, p_valueSize, p_key, p_value);
			}
		}
		else
		{
			char* l_hashMapEntryHeaderCursor = (char*)l_hashMapEntryHeader;
			*l_hashMapEntryHeaderCursor = 1;
			l_hashMapEntryHeaderCursor += sizeof(char);
			memcpy(l_hashMapEntryHeaderCursor, p_key, p_keySize);
			l_hashMapEntryHeaderCursor += p_keySize;
			memcpy(l_hashMapEntryHeaderCursor, p_value, p_valueSize);
		}
	}

	return 1;
};

char HashMap_GetValue(HashMap_PTR p_hashMap, size_t p_keySize, size_t p_valueSize, char* p_key, char** out_value)
{
	size_t l_keyIndex = HashMap_CalculateKeyIndex(p_hashMap, p_key, p_hashMap->Entries.Capacity);
	HashMapEntryHeader* l_hashMapEntryHeader = (HashMapEntryHeader*)HashMap_GetEntryMemory(&p_hashMap->Entries, p_keySize, p_valueSize, l_keyIndex);
	if (l_hashMapEntryHeader->IsOccupied)
	{
		char* l_existingKey = HashMapEntry_GetKey(l_hashMapEntryHeader);
		if (p_hashMap->HashFn(l_existingKey) == p_hashMap->HashFn(p_key))
		{
			*out_value = HashMapEntry_GetValue(l_hashMapEntryHeader, p_keySize);
			return 1;
		}

	}

	return 0;
};