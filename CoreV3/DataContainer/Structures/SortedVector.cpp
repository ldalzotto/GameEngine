#pragma message(__FILE__)

namespace _CoreV3
{

	void SortedVector_alloc(SortedVector* p_sortedVector, size_t p_elementSize, size_t p_initialCapcity, SortFunction p_function)
	{
		GenericArray_alloc(&p_sortedVector->GenericArray, p_elementSize, p_initialCapcity);
		p_sortedVector->Function = p_function;
	};
	
	void SortedVector_pushBack(SortedVector* p_sortedVector, void* p_value)
	{
		size_t l_insertIndex = 0;
		for (size_t i = 0; i < p_sortedVector->GenericArray.Size; i++)
		{
			short int l_compareValue = p_sortedVector->Function(p_value, GenericArray_at_unchecked(&p_sortedVector->GenericArray, i), NULL);
			if (l_compareValue >= 0)
			{
				l_insertIndex = i + 1;
			}
			else
			{
				break;
			}
		}

		GenericArray_isertAt_realloc(&p_sortedVector->GenericArray, p_value, 1, l_insertIndex);
	};
}