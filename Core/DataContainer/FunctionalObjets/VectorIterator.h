#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct Core_VectorIterator
{
	void* Core_VectorIterator_DataStructure;
	void* Current;
	size_t CurrentIndex;
	void* (*At)(void* p_dataStructure, size_t p_index);
	void* (*At_unchecked)(void* p_dataStructure, size_t p_index);
} Core_VectorIterator;

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_iterator);

