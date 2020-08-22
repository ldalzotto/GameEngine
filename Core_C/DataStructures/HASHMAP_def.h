#pragma once

#include "ARRAY_def.h"

typedef size_t(*HashMap_HashFn)(char* p_key);

typedef struct HashMap_TYP
{
	Array Entries;
	HashMap_HashFn HashFn;
}HashMap, * HashMap_PTR;