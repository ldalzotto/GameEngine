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
char Arr_Swap(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, size_t p_left, size_t p_right);

void Arr_CopyToRealloc(ARRAY_ELEMENTSIZE_PARAMETER_INTERFACE, Array_PTR p_target);


#define ARRAY_ALLOC_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_Alloc_##MethodNamePrefix##(ArrayTypePTR p_array, size_t p_initialCapacity) { Arr_Alloc(&p_array->array, sizeof(ArrayElementType), p_initialCapacity); };
#define ARRAY_FREE_FUNCTION(MethodNamePrefix, ArrayTypePTR) inline void Arr_Free_##MethodNamePrefix##(ArrayTypePTR p_array) { Arr_Free(&p_array->array); };
#define ARRAY_ZEROING_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_Zeroing_##MethodNamePrefix##(ArrayTypePTR p_array) { Arr_Zeroing(&p_array->array, sizeof(ArrayElementType)); };
#define ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_PushBackRealloc_##MethodNamePrefix##(ArrayTypePTR p_array, ArrayElementType* p_item) { Arr_PushBackRealloc(&p_array->array, sizeof(ArrayElementType), (char*)p_item); };
#define ARRAY_PUSHBACKREALLOC_FUNCTION_VALUE(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_PushBackRealloc_##MethodNamePrefix##(ArrayTypePTR p_array, ArrayElementType p_item) { Arr_PushBackRealloc(&p_array->array, sizeof(ArrayElementType), (char*)&p_item); };
#define ARRAY_ERASE_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_Erase_##MethodNamePrefix##(ArrayTypePTR p_array, size_t p_index) { Arr_Erase(&p_array->array, sizeof(ArrayElementType), p_index); };
#define ARRAY_SWAP_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_Swap_##MethodNamePrefix##(ArrayTypePTR p_array, size_t p_left, size_t p_right) { Arr_Swap(&p_array->array, sizeof(ArrayElementType), p_left, p_right); };
#define ARRAY_COPYTOREALLOC_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType) inline void Arr_CopyToRealloc_##MethodNamePrefix##(ArrayTypePTR p_source, ArrayTypePTR p_target) { Arr_CopyToRealloc(&p_source->array, sizeof(ArrayElementType), &p_target->array); };


ARRAY_ALLOC_FUNCTION(Char, Array_Char_PTR, char);
ARRAY_PUSHBACKREALLOC_FUNCTION_VALUE(Char, Array_Char_PTR, char);