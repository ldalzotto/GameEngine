#pragma once

#include <stdlib.h>

#include "DataStructures/GenericArray.h"

typedef Core_GenericArray Core_String;

void Core_string_alloc(struct Core_GenericArray* p_genericArray, size_t p_initialCapacity);