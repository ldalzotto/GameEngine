#pragma once

#include <stdlib.h>

#include "DataContainer/FunctionalObjets/VectorWriter.h"

typedef struct Core_Vector
{
	void* Memory;
	size_t Size;
	size_t Capacity;
	size_t ElementSize;

	Core_VectorWriter Writer;

} Core_Vector;

void Core_Vector_alloc(Core_Vector* p_vector, size_t p_initialCapacity);
void Core_Vector_free(Core_Vector* p_vector);

void* Core_Vector_at_unchecked(Core_Vector* p_vector, size_t p_index);

void Core_Vector_buildIterator(Core_Vector* p_vector, struct Core_VectorIterator* p_vectorIterator);