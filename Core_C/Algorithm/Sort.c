#include "Sort.h"

#include "Datastructures/ARRAY.h"
#include <math.h>

#include "Sort_alg.h"

SORT_QUICK_ALGORITHM(Int_Decreasing, Array_Int_PTR, int, SQA_ComparedElementValueExpression < *l_pivot, SQA_ComparedElementValueExpressionInvert > *l_pivot)

	/*
MethodPrefix
ArrayTypePTR, ElementType
Comparison function
Comparison function invert
*/

#if 0
size_t Sort_Quick_Partition(Array_Int_PTR p_arr, size_t p_begin, size_t p_end)
{
	int* l_pivot = &p_arr->Memory[(size_t)floorf(((float)(p_begin + p_end) * 0.5f))];
	size_t i = p_begin - 1; size_t j = p_end + 1;
	while (true)
	{
		do
		{
			i += 1;
		} while (p_arr->Memory[i] < *l_pivot);

		do
		{
			j -= 1;
		} while (p_arr->Memory[j] > * l_pivot);

		if (i >= j)
		{
			return j;
		}
		Arr_Swap_Unsafe(&p_arr->array, sizeof(int), i, j);
	}
};

void Sort_Quick_Between(Array_Int_PTR p_arr, size_t p_begin, size_t p_end)
{
	if (p_begin < p_end)
	{
		size_t l_pivot = Sort_Quick_Partition(p_arr, p_begin, p_end);
		Sort_Quick_Between(p_arr, p_begin, l_pivot);
		Sort_Quick_Between(p_arr, l_pivot + 1, p_end);
	}
};

void Sort_Quick(Array_Int_PTR p_arr)
{
	Sort_Quick_Between(p_arr, 0, p_arr->Size - 1);
};
#endif