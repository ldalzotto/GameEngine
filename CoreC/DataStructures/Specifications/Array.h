#pragma once

#include <stdlib.h>

#include "DataStructures/GenericArray.h"

void Core_Array_alloc(struct Core_GenericArray* p_genericArray, size_t p_elementSize, size_t p_initialCapacity);
struct Core_GenericArray Core_array_fromCStyle(void* p_cArray, size_t p_elementSize, size_t p_numberOfElements);