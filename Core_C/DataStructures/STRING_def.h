#pragma once

#include "ARRAY_def.h"

typedef struct STRING_TYP
{
	ARRAY_TYPE_DEFINITION(char)
} STRING, *STRING_PTR;

typedef struct STRINGSLICE_TYP
{
	char* Memory;
	size_t Begin;
	size_t End;
} STRINGSLICE, * STRINGSLICE_PTR;

typedef struct ARRAY_STRING_TYP
{
	ARRAY_TYPE_DEFINITION(STRING)
}ARRAY_STRING, * ARRAY_STRING_PTR;