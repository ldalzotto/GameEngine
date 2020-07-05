#pragma once

#include <stdlib.h>

#include "DataContainer/Structures/GenericArray.h"
#include "DataContainer/FunctionalObjets/VectorWriter.h"
#include "DataContainer/Algorithm/SortComparators.h"

typedef struct Core_SortedVector
{
	Core_GenericArray GenericArray;
	Core_VectorWriter Writer;
	void (*BuildIterator)(struct Core_SortedVector* p_vector, struct Core_VectorIterator* out_iterator);
	Core_SortElementComparatorWithUserObject SortComparator;
} Core_SortedVector;

void Core_SortedVector_alloc(Core_SortedVector* p_vector, size_t p_elementSize, size_t p_initialCapacity, Core_SortElementComparatorWithUserObject p_sortComparator);
void Core_SortedVector_free(Core_SortedVector* p_vector);