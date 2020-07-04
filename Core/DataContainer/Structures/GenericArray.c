#include "GenericArray.h"

#include <string.h>

void Core_GenericArray_alloc(Core_GenericArray* p_genericArray, size_t p_initialCapacity)
{
	p_genericArray->Capacity = p_initialCapacity;
	p_genericArray->Memory = malloc(Core_GenericArray_getTotalSize(p_genericArray));
	p_genericArray->Size = 0;
};

void Core_GenericArray_free(Core_GenericArray* p_genericArray)
{
	free(p_genericArray->Memory);
	p_genericArray->Memory = NULL;
	p_genericArray->Capacity = 0;
	p_genericArray->ElementSize = 0;
	p_genericArray->Size = 0;
};

size_t Core_GenericArray_getTotalSize(Core_GenericArray* p_genericArray)
{
	return p_genericArray->ElementSize * p_genericArray->Capacity;
};

size_t Core_GenericArray_getElementOffset(Core_GenericArray* p_genericArray, size_t p_index)
{
	return p_genericArray->ElementSize * p_index;
};

void* Core_GenericArray_at_unchecked(Core_GenericArray* p_genericArray, size_t p_index)
{
	return (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_index);
};

void* Core_GenericArray_at(Core_GenericArray* p_genericArray, size_t p_index)
{
	if (p_index >= p_genericArray->Size)
	{
		return NULL;
	}
	return Core_GenericArray_at_unchecked(p_genericArray, p_index);
};

void Core_GenericArray_resize(Core_GenericArray* p_genericArray, size_t p_newCapacity)
{
	if (p_newCapacity > p_genericArray->Capacity)
	{
		void* l_newMemory = realloc(p_genericArray->Memory, p_newCapacity * p_genericArray->ElementSize);
		if (l_newMemory)
		{
			p_genericArray->Memory = l_newMemory;
			p_genericArray->Capacity = p_newCapacity;
		}
	}
}

void Core_GenericArray_pushBack_realloc(Core_GenericArray* p_genericArray, void* p_value)
{
	if (p_genericArray->Size >= p_genericArray->Capacity)
	{
		Core_GenericArray_resize(p_genericArray, p_genericArray->Capacity == 0 ? 1 : (p_genericArray->Capacity * 2));
		Core_GenericArray_pushBack_realloc(p_genericArray, p_value);
	}
	else
	{
		void* p_targetMemory = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_genericArray->Size);
		memcpy(p_targetMemory, p_value, p_genericArray->ElementSize);
		p_genericArray->Size += 1;
	}
}


void Core_GenericArray_swap(Core_GenericArray* p_genericArray, size_t p_left, size_t p_right)
{
	if (p_left >= p_genericArray->Size || p_right >= p_genericArray->Size) { exit(EXIT_FAILURE); /*throw std::runtime_error("Vector_swap : Out of range.");*/ }
	if (p_left > p_right) { exit(EXIT_FAILURE); /*throw std::out_of_range("Vector_swap : invalid indices.");*/ }
	if (p_left == p_right) { return; }


	char* l_leftMemoryTarget = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_left);
	char* l_rightMemoryTarget = (char*)p_genericArray->Memory + Core_GenericArray_getElementOffset(p_genericArray, p_right);

	for (size_t i = 0; i < p_genericArray->ElementSize; i++)
	{
		char l_rightTmp = l_rightMemoryTarget[i];
		l_rightMemoryTarget[i] = l_leftMemoryTarget[i];
		l_leftMemoryTarget[i] = l_rightTmp;
	}
};