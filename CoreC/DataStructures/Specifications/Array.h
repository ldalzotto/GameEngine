#pragma once

#include <stdlib.h>

#include "DataStructures/GenericArray.h"

typedef struct Core_Array
{
	Core_GenericArray GenericArray;
} Core_Array;

struct Core_GenericArray Core_array_fromCStyle(void* p_cArray, size_t p_elementSize, size_t p_numberOfElements);