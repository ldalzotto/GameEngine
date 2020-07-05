#pragma once

#include <stdlib.h>

#include "DataContainer/Structures/GenericArray.h"
#include "DataContainer/FunctionalObjets/VectorWriter.h"

typedef struct Core_Vector
{
	Core_GenericArray GenericArray;
	Core_VectorWriter Writer;
	void (*BuildIterator)(struct Core_Vector* p_vector, struct Core_VectorIterator* out_iterator);
} Core_Vector;

void Core_Vector_alloc(Core_Vector* p_vector, size_t p_elementSize, size_t p_initialCapacity);
void Core_Vector_free(Core_Vector* p_vector);

// void Core_Vector_buildIterator(Core_Vector* p_vector, struct Core_VectorIterator* p_vectorIterator);