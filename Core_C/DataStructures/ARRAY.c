#include "DataStructures/ARRAY.h"
#include <stdlib.h>
#include <string.h>

#include "Error/ErrorHandler.h"

inline void memcpy_arraysafe_dist(char* p_dist, const char* p_source, const size_t p_size, const Array_PTR p_distArray, const size_t p_distArrayElementSize)
{
#if BUFFER_OVERFLOW_INTERNAL_CONDITION
	HandleBufferOverflowMemoryWrite(p_dist, p_size, p_distArray->Memory, p_distArray->Capacity * p_distArrayElementSize);
#endif
	memcpy(p_dist, p_source, p_size);
};

inline void memmove_arraysafe_dist(char* p_dist, const char* p_source, const size_t p_size, const Array_PTR p_distArray, const size_t p_distArrayElementSize)
{
#if BUFFER_OVERFLOW_INTERNAL_CONDITION
	HandleBufferOverflowMemoryWrite(p_dist, p_size, p_distArray->Memory, p_distArray->Capacity * p_distArrayElementSize);
#endif
	memmove(p_dist, p_source, p_size);
};

inline void memset_arraysafe_dist(char* p_dist, int p_val, const size_t p_size, const Array_PTR p_distArray, const size_t p_distArrayElementSize)
{
#if BUFFER_OVERFLOW_INTERNAL_CONDITION
	HandleBufferOverflowMemoryWrite(p_dist, p_size, p_distArray->Memory, p_distArray->Capacity * p_distArrayElementSize);
#endif
	memset(p_dist, p_val, p_size);
};

#define ARRAY_ELEMENTSIZE_PARAMETER_INPUT p_array, p_elementSize

inline size_t Arr_GetCapacitySize(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE)
{
	return p_elementSize * p_array->Capacity;
}

inline size_t Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_index)
{
	return p_elementSize * p_index;
};

void Arr_Alloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_initialCapacity)
{
	p_array->Capacity = p_initialCapacity;
	p_array->Memory = malloc(Arr_GetCapacitySize(p_array, p_elementSize));
	p_array->Size = 0;
};

void Arr_Free(Array_PTR p_array)
{
	free(p_array->Memory);
	p_array->Memory = NULL;
	p_array->Capacity = 0;
	p_array->Size = 0;
};

void Arr_Zeroing(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE)
{
	memset_arraysafe_dist(p_array->Memory, 0, Arr_GetCapacitySize(p_array, p_elementSize), p_array, p_elementSize);
};

void Arr_Clear(Array_PTR p_array)
{
	p_array->Size = 0;
};

char Arr_Resize(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_newCapacity)
{
	if (p_newCapacity > p_array->Capacity)
	{
		void* l_newMemory = realloc(p_array->Memory, p_newCapacity * p_elementSize);
		if (l_newMemory != NULL)
		{
			p_array->Memory = l_newMemory;
			p_array->Capacity = p_newCapacity;
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

char Arr_PushBackRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value)
{
	if (p_array->Size >= p_array->Capacity)
	{
		if (Arr_Resize(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_array->Capacity == 0 ? 1 : (p_array->Capacity * 2)))
		{
			return 1;
		};
		Arr_PushBackRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_value);
	}
	else
	{
		void* p_targetMemory = (char*)p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_array->Size);
		memcpy_arraysafe_dist(p_targetMemory, p_value, p_elementSize, p_array, p_elementSize);
		p_array->Size += 1;
	}

	return 0;
}

char Arr_PushBackNoRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value)
{
	if (p_array->Size >= p_array->Capacity)
	{
		return 1;
	}

	HANDLE_ERR_SILENT(Arr_PushBackRealloc(p_array, p_elementSize, p_value));
	return 0;
};

char Arr_InsertAtRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value, size_t p_elementNb, size_t p_index)
{
#ifndef NDEBUG
	if (p_index > p_array->Size)
	{
		return 1;
	}
#endif

	if (p_array->Size + p_elementNb > p_array->Capacity)
	{
		Arr_Resize(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_array->Capacity == 0 ? 1 : (p_array->Capacity * 2));
		Arr_InsertAtRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_value, p_elementNb, p_index);
	}
	else
	{
		void* l_initialElement = (char*)p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_index);
		// If we insert between existing elements, we move down memory to give space for new elements
		if (p_array->Size - p_index > 0)
		{
			void* l_targetElement = (char*)p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_index + p_elementNb);
			memmove_arraysafe_dist(l_targetElement, l_initialElement, p_elementSize * (p_array->Size - p_index), p_array, p_elementSize);
		}
		memcpy_arraysafe_dist(l_initialElement, p_value, p_elementSize * p_elementNb, p_array, p_elementSize);
		p_array->Size += p_elementNb;
	}

	return 0;
};

char Arr_InsertAtNoRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value, size_t p_elementNb, size_t p_index)
{
	if ((p_array->Size + p_elementNb) >= p_array->Capacity)
	{
		return 1;
	}

	HANDLE_ERR_SILENT(Arr_InsertAtRealloc(p_array, p_elementSize, p_value, p_elementNb, p_index));
	return 0;
};

char Arr_Erase(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_index)
{
#ifndef NDEBUG
	if (p_index >= p_array->Size)
	{
		return 1;
	}
#endif

	// If we are not erasing the last element, then we move memory. Else, we have nothing to do.
	if (p_index + 1 != p_array->Size)
	{
		void* p_targetMemory = (char*)p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_index);
		memmove_arraysafe_dist(p_targetMemory, (char*)p_targetMemory + p_elementSize, (p_array->Size - p_index - 1) * p_elementSize, p_array, p_elementSize);
	}

	p_array->Size -= 1;
	return 0;
}

char Arr_Swap(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_left, size_t p_right)
{
#ifndef NDEBUG
	if (p_left >= p_array->Size || p_right >= p_array->Size) { printf("Core_GenericArray_swap : out_of_range"); return 1; }
#endif
	if (p_left == p_right) { return 0; }

	char* l_leftMemoryTarget = p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_left);
	char* l_rightMemoryTarget = p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, p_right);

	for (size_t i = 0; i < p_elementSize; i++)
	{
		char l_rightTmp = l_rightMemoryTarget[i];
		l_rightMemoryTarget[i] = l_leftMemoryTarget[i];
		l_leftMemoryTarget[i] = l_rightTmp;
	}

	return 0;
};

void Arr_CopyToRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, Array_PTR p_target)
{
	size_t l_sizeToCopy = Arr_GetCapacitySize(p_array, p_elementSize);
	if (l_sizeToCopy > 0)
	{
		if (Arr_GetCapacitySize(p_array, p_elementSize) > Arr_GetCapacitySize(p_target, p_elementSize))
		{
			Arr_Resize(p_target, p_elementSize, Arr_GetCapacitySize(p_array, p_elementSize));
		}
	
		memcpy_arraysafe_dist(p_target->Memory, p_array->Memory, l_sizeToCopy, p_target, p_elementSize);
		p_target->Size = p_array->Size;
	}
};

void Arr_BuildIterator(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, ArrayIterator_PTR p_iter)
{
	p_iter->Array = p_array;
	p_iter->Current = p_array->Memory - p_elementSize;
	p_iter->CurrentIndex = -1;
};

bool Iter_MoveNext(ArrayIterator_PTR p_iterator, size_t p_elementSize)
{
	p_iterator->CurrentIndex += 1;
	if (p_iterator->CurrentIndex < p_iterator->Array->Size)
	{
		p_iterator->Current += p_elementSize;
		return true;
	}
	else
	{
		return false;
	}
};




#include "Algorithm/Sort.h"


size_t Sort_Min(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_startIndex, SortFunction p_sortFunction, void* p_closure)
{
	size_t l_sortIteratrionIndex = p_startIndex;
	size_t l_minIndex = p_startIndex;
	void* l_minValue = NULL;

	if (p_array->Size > 0)
	{
		l_minValue = p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, l_minIndex);
	}

	for (; l_sortIteratrionIndex < p_array->Size; l_sortIteratrionIndex++)
	{
		void* l_comparedValue = p_array->Memory + Arr_GetElementOffset(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, l_sortIteratrionIndex);
		short int l_compareValue = p_sortFunction(l_minValue, l_comparedValue, p_closure);
		if (l_compareValue >= 0)
		{
			l_minIndex = l_sortIteratrionIndex;
			l_minValue = l_comparedValue;
		}
	}

	return l_minIndex;
};

void Sort_SelectionSort(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, SortFunction p_sortFunction, void* p_closure)
{
	for (size_t i = 0; i < p_array->Size; i++)
	{
		size_t l_minIndex = Sort_Min(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, i, p_sortFunction, p_closure);
		if (l_minIndex != i)
		{
			Arr_Swap(ARRAY_ELEMENTSIZE_PARAMETER_INPUT, l_minIndex, i);
		}
	}
};