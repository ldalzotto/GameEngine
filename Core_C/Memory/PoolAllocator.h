#pragma once

#include "PoolAllocator_def.h"

#include "DataStructures/ARRAY.h"

ARRAY_ALLOC_FUNCTION(PoolAllocatorFreeItemHeader, Array_PoolAllocatorFreeItemHeader_PTR, PoolAllocatorFreeItemHeader);
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(PoolAllocatorFreeItemHeader, Array_PoolAllocatorFreeItemHeader_PTR, PoolAllocatorFreeItemHeader);


void PoolAllocator_Alloc(PoolAllocator_PTR p_poolAllocator, size_t p_elementSize, size_t p_initialCapacity);
void PoolAllocator_Free(PoolAllocator_PTR p_poolAllocator);

void PoolAllocator_AllocElement(PoolAllocator_PTR p_poolAllocator, const size_t p_elementSize, size_t* out_handle);
void PoolAllocator_FreeElement(PoolAllocator_PTR p_poolAllocator, const size_t p_handle);
void PoolAllocator_FreeElements(PoolAllocator_PTR p_poolAllocator, const Array_PoolAllocatorFreeItemHeader_PTR p_handles);

#define POOLALLOCATOR_ALLOC_FUNCTION(MethodNamePrefix, PoolAllocatorTypePtr, ArrayElementType) inline PoolAllocator_Alloc_##MethodNamePrefix(PoolAllocatorTypePtr p_poolAllocator, size_t p_initialCapacity) { PoolAllocator_Alloc(&p_poolAllocator->allocator, sizeof(ArrayElementType), p_initialCapacity); };
#define POOLALLOCATOR_ALLOCELEMENT_FUNCTION(MethodNamePrefix, PoolAllocatorTypePtr, ArrayElementType, HandleTypePTR) inline PoolAllocator_AllocElement_##MethodNamePrefix(PoolAllocatorTypePtr p_poolAllocator, HandleTypePTR out_handle){ PoolAllocator_AllocElement(&p_poolAllocator->allocator, sizeof(ArrayElementType), (size_t*)out_handle); };
#define POOLALLOCATOR_FREEELEMENT_FUNCTION(MethodNamePrefix, PoolAllocatorTypePtr, HandleType) inline PoolAllocator_FreeElement_##MethodNamePrefix(PoolAllocatorTypePtr p_poolAllocator, HandleType p_handle){ PoolAllocator_FreeElement(&p_poolAllocator->allocator, p_handle.Handle); };
#define POOLALLOCATOR_FREEELEMENTS_FUNCTION(MethodNamePrefix, PoolAllocatorTypePtr, HandleArrayTypePtr) inline PoolAllocator_FreeElements_##MethodNamePrefix(PoolAllocatorTypePtr p_poolAllocator, HandleArrayTypePtr p_handles){ PoolAllocator_FreeElements(&p_poolAllocator->allocator, (const Array_PoolAllocatorFreeItemHeader_PTR)p_handles); };
