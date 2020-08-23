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

#define ARRAY_ALLOC_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_Alloc_##MethodNamePrefix##(ArrayTypePTR p_array, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(ArrayElementType), p_initialCapacity); };
#define ARRAY_FREE_FUNCTION(MethodNamePrefix, ArrayTypePTR) inline void Arr_Free_##MethodNamePrefix##(ArrayTypePTR p_array) { Arr_Free((Array_PTR)p_array); };
#define ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_PushBackRealloc_##MethodNamePrefix##(ArrayTypePTR p_array, ArrayElementType* p_item) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(ArrayElementType), (char*)p_item); };
#define ARRAY_PUSHBACKREALLOC_FUNCTION_VALUE(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_PushBackRealloc_##MethodNamePrefix##(ArrayTypePTR p_array, ArrayElementType p_item) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(ArrayElementType), (char*)&p_item); };
#define ARRAY_ERASE_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_Erase_##MethodNamePrefix##(ArrayTypePTR p_array, size_t p_index) { Arr_Erase((Array_PTR)p_array, sizeof(ArrayElementType), p_index); };
