#pragma once

#include <stdlib.h>

typedef struct Core_VectorAccessor
{
    void* (*At)(struct Core_GenericArray* p_dataStructure, size_t p_index);
    void* (*At_unchecked)(struct Core_GenericArray* p_dataStructure, size_t p_index);
} Core_VectorAccessor;