#pragma once

#include <stdlib.h>

#include "DataStructures/GenericArray.h"
#include "Functional/Sort/ElementSorter.h"

typedef struct Core_SortedVector
{
	Core_GenericArray GenericArray;
	Core_elementSort_function SortFunction;
} Core_SortedVector;

void Core_SortedVector_alloc(Core_SortedVector* p_sortedVector, size_t p_elementSize, size_t p_initialCapacity, Core_elementSort_function p_sortFunction);
void Core_SortedVector_free(Core_SortedVector* p_sortedVector);

Core_SortedVector Core_SortedVector_deepCopy(Core_SortedVector* p_sortedVector);