#pragma once

#include "ErrorHandling/ReturnCodes.h"
#include "DataStructures/GenericArray.h"
#include "Functional/Equals/Equals.h"

typedef struct Core_LinearMap
{
	Core_GenericArray GenericArray;
	Core_Equals_function EqualsFunction;
} Core_LinearMap;

void Core_LinearMap_alloc(Core_LinearMap* p_linearMap, size_t p_elementSize, size_t p_initialCapacity, Core_Equals_function p_equalsFunction);
void Core_LinearMap_free(Core_LinearMap* p_linearMap);

Core_ReturnCodes Core_LinearMap_pushBack_realloc(Core_LinearMap* p_linearMap, void* p_element);