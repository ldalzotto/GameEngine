#pragma once

#include "ErrorHandling/ReturnCodes.h"
#include "DataStructures/GenericArray.h"
#include "Functional/Equals/Equals.h"
#include "Functional/Sort/ElementSorter.h"
#include "Functional/Callback/Callback.h"

typedef struct Core_SortedLinearMap
{
	Core_GenericArray GenericArray;
	Core_Equals_function EqualsFunction;
	Core_elementSort_function SortFunction;
} Core_SortedLinearMap;

void Core_SortedLinearMap_alloc(Core_SortedLinearMap* p_sortedLinearMap, size_t p_elementSize, size_t p_initialCapacity, Core_elementSort_function p_sortFunction, Core_Equals_function p_equalsFunction);
void Core_SortedLinearMap_free(Core_SortedLinearMap* p_sortedLinearMap);

Core_ReturnCodes Core_SortedLinearMap_pushBack_realloc(Core_SortedLinearMap* p_sortedLinearMap, void* p_value, void** p_out_insertedElement);
bool Core_SortedLinearMap_find(Core_SortedLinearMap* p_sortedLinearMap, void* p_comparedElement, Core_VectorIterator* p_out_vectorIterator);
Core_SortedLinearMap Core_SortedLinearMap_deepCopy(Core_SortedLinearMap* p_source);