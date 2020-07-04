#pragma once

#include <stdlib.h>

#include "DataContainer/Structures/GenericArray.h"
#include "DataContainer/FunctionalObjets/VectorWriter.h"

typedef struct Core_SortedVector
{
	Core_GenericArray GenericArray;
	Core_VectorWriter Writer;
} Core_SortedVector;

void Core_SortedVector_alloc(Core_SortedVector* p_vector, size_t p_initialCapacity);
void Core_SortedVector_free(Core_SortedVector* p_vector);
void Core_SortedVector_buildIterator(Core_SortedVector* p_vector, struct Core_VectorIterator* p_vectorIterator);