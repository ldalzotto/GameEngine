#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct Core_VectorIterator
{
	void* Core_VectorIterator_UserObject;
	void* Current;
	size_t CurrentIndex;
	void* (*GetElement)(void* p_userObject, size_t p_index);
} Core_VectorIterator;

bool Core_VectorIterator_moveNext(Core_VectorIterator* p_iterator);