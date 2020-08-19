#pragma once

#include "ARRAY_def.h"

#include <stdbool.h>

#define ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE ARRAY_PTR p_array, size_t p_elementSize

void Arr_Alloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_initialCapacity);
void Arr_Free(ARRAY_PTR p_array);
void Arr_Zeroing(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE);

void Arr_PushBackRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value);
char Arr_Erase(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_index);

void Arr_BuildIterator(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, ARRAY_ITERATOR_PTR p_iter);

bool Iter_MoveNext(ARRAY_ITERATOR_PTR p_iterator, size_t p_elementSize);
