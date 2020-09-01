#include "Memory/PoolAllocator.h"

#include <stdio.h>
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/VectorStructuresC_def.h"

#include "Clock/Clock.h"


typedef struct Vector3f_PoolAllocator_TYP
{
	POOLALLOCATOR_TYPE_DEFINITION(Array_Vector3f)
}Vector3f_PoolAllocator, * Vector3f_PoolAllocator_PTR;

typedef struct Vector3f_HANDLE_TYP
{
	POOLALLOCATOR_HANDLE_TYPE_DEFINITION()
}Vector3f_HANDLE, *Vector3f_HANDLE_PTR;

POOLALLOCATOR_ALLOC_FUNCTION(Vector3f, Vector3f_PoolAllocator_PTR, Vector3f);
POOLALLOCATOR_ALLOCELEMENT_FUNCTION(Vector3f, Vector3f_PoolAllocator_PTR, Vector3f, Vector3f_HANDLE_PTR);
POOLALLOCATOR_FREEELEMENT_FUNCTION(Vector3f, Vector3f_PoolAllocator_PTR, Vector3f_HANDLE);

int main()
{

	Array l_sizetArray; Arr_Alloc(&l_sizetArray, sizeof(size_t), 1000000);
	Vector3f_PoolAllocator l_vect3Allocator;
	PoolAllocator_Alloc_Vector3f(&l_vect3Allocator, 1000000);
	{
		for (size_t i = 0; i< 1000000; i++)
		{
			Vector3f_HANDLE l_allocatedHandle;
			PoolAllocator_AllocElement_Vector3f(&l_vect3Allocator, &l_allocatedHandle);
			Arr_PushBackRealloc(&l_sizetArray, sizeof(size_t), (char*)&l_allocatedHandle);
		}

		printf("BEGIN \n");

		TimeClockPrecision l_begin = Clock_currentTime_mics();
		
		for (size_t i = 0; i < l_sizetArray.Size; i++)
		{
			l_vect3Allocator.array.Memory[i].x += 1;
		}

		TimeClockPrecision l_end = Clock_currentTime_mics();

		printf("%d \n", l_end - l_begin);

		l_begin = Clock_currentTime_mics();
		for (size_t i = 0; i < l_sizetArray.Size; i++)
		{
			l_vect3Allocator.array.Memory[((size_t*)l_sizetArray.Memory)[i]].x += 1;
		}
	
		l_end = Clock_currentTime_mics();
		printf("%d \n", l_end - l_begin);
	}
	PoolAllocator_Free(&l_vect3Allocator.allocator);
	Arr_Free(&l_sizetArray);
}