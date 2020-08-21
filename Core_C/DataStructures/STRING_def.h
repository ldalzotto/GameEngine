#pragma once

#include "ARRAY_def.h"

typedef struct String_TYP
{
	ARRAY_TYPE_DEFINITION(char)
} String, *String_PTR;

typedef struct StringSLICE_TYP
{
	char* Memory;
	size_t Begin;
	size_t End;
} StringSLICE, * StringSLICE_PTR;

typedef struct Array_String_TYP
{
	ARRAY_TYPE_DEFINITION(String)
}Array_String, * Array_String_PTR;