#pragma once

#include "Datastructures/ARRAY.h"
#include <math.h>

//https://en.wikipedia.org/wiki/Quicksort

#define SQA_ComparedElementValueExpression p_arr->Memory[i]
#define SQA_ComparedElementValueExpressionInvert p_arr->Memory[j]

#define SORT_QUICK_ALGORITHM(MethodPrefix, ArrayTypePTR, ElementType, ComparisonFunction, ComparisonFunctionInvert) \
size_t Sort_Quick_Partition_##MethodPrefix##(ArrayTypePTR p_arr, size_t p_begin, size_t p_end) \
{ \
ElementType* l_pivot = &p_arr->Memory[(size_t)floorf(((float)(p_begin + p_end) * 0.5f))]; \
size_t i = p_begin - 1; size_t j = p_end + 1; \
while (true) \
{ \
	do \
	{ \
		i += 1; \
	} while (ComparisonFunction); \
 \
	do \
	{ \
		j -= 1; \
	} while (ComparisonFunctionInvert); \
 \
	if (i >= j) \
	{ \
		return j; \
	} \
	Arr_Swap_Unsafe(&p_arr->array, sizeof(ElementType), i, j); \
} \
}; \
 \
void Sort_Quick_Between_##MethodPrefix##(ArrayTypePTR p_arr, size_t p_begin, size_t p_end) \
{ \
	if (p_begin < p_end) \
	{ \
		size_t l_pivot = Sort_Quick_Partition_##MethodPrefix##(p_arr, p_begin, p_end); \
		Sort_Quick_Between_##MethodPrefix##(p_arr, p_begin, l_pivot); \
		Sort_Quick_Between_##MethodPrefix##(p_arr, l_pivot + 1, p_end); \
	} \
}; \
 \
void Sort_Quick_##MethodPrefix##(ArrayTypePTR p_arr) \
{ \
if (p_arr->Size > 0) \
{ \
	Sort_Quick_Between_##MethodPrefix##(p_arr, 0, p_arr->Size - 1); \
} \
};
