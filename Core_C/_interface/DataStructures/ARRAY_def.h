#pragma once

#include <stdlib.h>

typedef struct ARRAY_TYP
{
	char* Memory;
	size_t Size;
	size_t Capacity;
} ARRAY, *ARRAY_PTR;

#define ARRAY_TYPE_DEFINITION(ArrayElement) \
ArrayElement* Memory; \
size_t Size; \
size_t Capacity;

typedef struct ARRAY_ITERATOR_TYP
{
	ARRAY_PTR Array;
	char* Current;
	size_t CurrentIndex;
} ARRAY_ITERATOR, * ARRAY_ITERATOR_PTR;

#define ARRAY_ITERATOR_DEFINITION(ArrayType, ArrayElement) \
ArrayType* Array; \
ArrayElement* Current; \
size_t CurrentIndex;
