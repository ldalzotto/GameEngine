#include "PoolAllocator.h"

void PoolAllocator_Alloc(PoolAllocator_PTR p_poolAllocator, size_t p_elementSize, size_t p_initialCapacity)
{
	Arr_Alloc(&p_poolAllocator->array, p_elementSize, p_initialCapacity);
	Arr_Alloc_PoolAllocatorFreeItemHeader(&p_poolAllocator->FreeBlocks, 0);
};

void PoolAllocator_Free(PoolAllocator_PTR p_poolAllocator)
{
	Arr_Free(&p_poolAllocator->array);
	Arr_Free(&p_poolAllocator->FreeBlocks.array);
};

void PoolAllocator_AllocElement(PoolAllocator_PTR p_poolAllocator, const size_t p_elementSize, size_t* out_handle)
{
	if (p_poolAllocator->FreeBlocks.Size > 0)
	{
		PoolAllocatorFreeItemHeader_PTR l_freeBlock = &p_poolAllocator->FreeBlocks.Memory[p_poolAllocator->FreeBlocks.Size - 1];
		*out_handle = l_freeBlock->FreeMemoryIndex;
		p_poolAllocator->FreeBlocks.Size -= 1;
	}
	else
	{
		Arr_Resize(&p_poolAllocator->array, p_elementSize, p_poolAllocator->array.Size + 1);
		p_poolAllocator->array.Size += 1;
		*out_handle = p_poolAllocator->array.Size - 1;
	}
};

void PoolAllocator_FreeElement(PoolAllocator_PTR p_poolAllocator, const size_t p_handle)
{
	PoolAllocatorFreeItemHeader l_header = {p_handle};
	Arr_PushBackRealloc_PoolAllocatorFreeItemHeader(&p_poolAllocator->FreeBlocks, &l_header);
};