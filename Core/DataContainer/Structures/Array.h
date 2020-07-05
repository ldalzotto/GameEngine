#pragma once

#include <stdlib.h>

#include "DataContainer/Structures/GenericArray.h"
#include "DataContainer/FunctionalObjets/VectorWriter.h"

typedef struct Core_Array
{
	Core_GenericArray GenericArray;
	Core_VectorWriter Writer;
	void (*BuildIterator)(struct Core_Array* p_array, struct Core_VectorIterator* out_iterator);
} Core_Array;

void Core_Array_alloc(Core_Array* p_array, size_t p_elementSize, size_t p_initialCapacity);
void Core_Array_free(Core_Array* p_array);

// void Core_Vector_buildIterator(Core_Vector* p_vector, struct Core_VectorIterator* p_vectorIterator);