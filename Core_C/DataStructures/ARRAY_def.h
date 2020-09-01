#pragma once

#include <stdlib.h>

typedef struct Array_TYP
{
	char* Memory;
	size_t Size;
	size_t Capacity;
} Array, *Array_PTR;

#define ARRAY_TYPE_DEFINITION(ArrayElement) \
union \
{ \
	struct { Array array; }; \
	struct \
	{ \
		ArrayElement* Memory; \
		size_t Size; \
		size_t Capacity; \
	}; \
};