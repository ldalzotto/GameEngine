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

typedef struct Array_Int_TYP
{
	ARRAY_TYPE_DEFINITION(int)
}Array_Int, * Array_Int_PTR;

typedef struct Array_Float_TYP
{
	ARRAY_TYPE_DEFINITION(float)
}Array_Float, * Array_Float_PTR;