#pragma once

#include "DataStructures/ARRAY_def.h"

typedef short int(*SortFunction)(void* p_sourceObject, void* p_comparedObject, void* p_closure);

size_t Sort_Min(Array_PTR p_vector, size_t p_elementSize, size_t p_startIndex, SortFunction p_sortFunction, void* p_closure);
void Sort_SelectionSort(Array_PTR p_vector, size_t p_elementSize, SortFunction p_sortFunction, void* p_closure);
void Sort_Quick_Int_Decreasing(Array_Int_PTR p_arr);

#define SORT_SELECTION_FUNCTION(MethodNamePrefix, ArrayTypePTR, ArrayElementType, ClosureTypePTR) inline void Sort_SelectionSort_##MethodNamePrefix##(ArrayTypePTR p_vector, short int(*p_sortFunction)(ArrayElementType*, ArrayElementType*, ClosureTypePTR), ClosureTypePTR p_closure) { Sort_SelectionSort(&p_vector->array, sizeof(ArrayElementType), (SortFunction)p_sortFunction, (void*)p_closure); }
