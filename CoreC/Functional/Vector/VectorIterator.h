#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct Core_VectorIterator
{
	struct Core_GenericArray* Array;
	void* Current;
	size_t CurrentIndex;
} Core_VectorIterator;

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_vectorIterator);