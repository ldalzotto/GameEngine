#pragma once

#include "DataStructures/ARRAY_def.h"

typedef struct PoolAllocatorFreeItemHeader_TYP
{
	size_t FreeMemoryIndex;
}PoolAllocatorFreeItemHeader, * PoolAllocatorFreeItemHeader_PTR;

typedef struct Array_PoolAllocatorFreeItemHeader_TYP
{
	ARRAY_TYPE_DEFINITION(PoolAllocatorFreeItemHeader)
}Array_PoolAllocatorFreeItemHeader, * Array_PoolAllocatorFreeItemHeader_PTR;

typedef struct PoolAllocator_TYP
{
	Array array;
	Array_PoolAllocatorFreeItemHeader FreeBlocks;
}PoolAllocator, * PoolAllocator_PTR;

#define POOLALLOCATOR_TYPE_DEFINITION(ArrayType) \
union \
{ \
	struct { PoolAllocator allocator; }; \
	struct \
	{ \
		ArrayType array; \
		Array_PoolAllocatorFreeItemHeader FreeBlocks; \
	}; \
};

#define POOLALLOCATOR_HANDLE_TYPE_DEFINITION() \
size_t Handle;