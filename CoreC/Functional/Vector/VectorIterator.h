#pragma once

#include <stdlib.h>
#include <stdbool.h>

struct Core_VectorIterator;

typedef struct Core_VectorIterator_function
{
	bool(*MoveNext)(struct Core_VectorIterator* p_iterator);
} Core_VectorIterator_function;

typedef struct Core_VectorIterator
{
	struct Core_GenericArray* Array;
	void* Current;
	size_t CurrentIndex;
	const struct Core_VectorIterator_function* Functions;
} Core_VectorIterator;