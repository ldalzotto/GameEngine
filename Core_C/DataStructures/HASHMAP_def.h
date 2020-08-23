#pragma once


typedef size_t(*HashMap_HashFn)(char* p_key);

typedef struct HashMapEntryHeader_TYP
{
	char IsOccupied;
} HashMapEntryHeader, * HashMapEntryHeader_PTR;

typedef struct HashMap_TYP
{
	char* Entries;
	size_t Capacity;
	HashMap_HashFn HashFn;
}HashMap, * HashMap_PTR;

typedef struct HashMapEntryLayout_TYP
{
	size_t KeyOffset;
	size_t ValueOffset;

	size_t TotalSize;
	size_t KeySize;
	size_t ValueSize;
}HashMapEntryLayout, *HashMapEntryLayout_PTR;

#define HASHMAPENTRY_TYPE_DEFIINITION(KeyType, ValueType) \
HashMapEntryHeader Header; \
KeyType Key; \
ValueType Value;

#define HASHMAP_TYPE_DEFINITION(EntryType) \
EntryType* Entries; \
size_t Capacity; \
HashMap_HashFn HashFn;