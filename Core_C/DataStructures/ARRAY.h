#pragma once

#include "ARRAY_def.h"

#include <stdbool.h>

#define ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE Array_PTR p_array, size_t p_elementSize

void Arr_Alloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_initialCapacity);
void Arr_Free(Array_PTR p_array);

void Arr_Zeroing(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE);
void Arr_Clear(Array_PTR p_array);
char Arr_Resize(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_newCapacity);

char Arr_PushBackRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value);
char Arr_PushBackNoRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value);

char Arr_InsertAtRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value, size_t p_elementNb, size_t p_index);
char Arr_InsertAtNoRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, char* p_value, size_t p_elementNb, size_t p_index);

char Arr_Erase(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_index);

void Arr_BuildIterator(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, ArrayIterator_PTR p_iter);

bool Iter_MoveNext(ArrayIterator_PTR p_iterator, size_t p_elementSize);
