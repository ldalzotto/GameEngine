#pragma once

#include <stdlib.h>

#include "Error/ReturnCodes.h"

typedef struct Core_GenericArray
{
	void* Memory;
	size_t Size;
	size_t Capacity;
	size_t ElementSize;
} Core_GenericArray;

void Core_GenericArray_alloc(Core_GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity);
void Core_GenericArray_free(Core_GenericArray* p_genericArray);

void* Core_GenericArray_at_unchecked(Core_GenericArray* p_genericArray, size_t p_index);
void* Core_GenericArray_at(Core_GenericArray* p_genericArray, size_t p_index);

void Core_GenericArray_pushBack_realloc(Core_GenericArray* p_genericArray, void* p_value);
Core_ReturnCodes Core_GenericArray_swap(Core_GenericArray* p_genericArray, size_t p_left, size_t p_right);

Core_ReturnCodes Core_GenericArray_isertAt_realloc(Core_GenericArray* p_genericArray, void* p_value, size_t p_elementNb, size_t p_index);
Core_ReturnCodes Core_GenericArray_isertArrayAt_realloc(Core_GenericArray* p_genericArray, Core_GenericArray* p_insertedArray, size_t p_index);