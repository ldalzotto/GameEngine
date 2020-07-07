#pragma message(__FILE__)

namespace _CoreV3
{
	template <class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	struct ElementSorterT
	{
		short int (*Function)(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right, USER_OBJECT* p_userObject);
		USER_OBJECT* UserObject;
	};
	

	template <class DATA_CONTAINER, class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	SOURCE_ELEMENT* Min(DATA_CONTAINER* p_genericArray, ElementSorterT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT>* p_elementSorter)
	{
		SOURCE_ELEMENT* l_minValue = NULL;
		if (p_genericArray->Size > 0)
		{
			l_minValue = At_unchecked(p_genericArray, 0);

			for (size_t i = 1; i < p_genericArray->Size; i++)
			{
				SOURCE_ELEMENT* l_comparedValue = At_unchecked(p_genericArray, i);
				short int l_compareValue = p_elementSorter->Function(l_minValue, l_comparedValue, p_elementSorter->UserObject);
				if (l_compareValue >= 0)
				{
					l_minValue = l_comparedValue;
				}
			}
		}
		return l_minValue;
	};

	template <class DATA_CONTAINER, class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	size_t MinIndex(DATA_CONTAINER* p_genericArray, ElementSorterT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT>* p_elementSorter, size_t p_beginIndex = 0)
	{
		SOURCE_ELEMENT* l_minValue = NULL;
		size_t l_minIndex = p_beginIndex;

		if (p_genericArray->Size > 0)
		{
			l_minValue = At_unchecked(p_genericArray, l_minIndex);

			for (size_t i = p_beginIndex + 1; i < p_genericArray->Size; i++)
			{
				SOURCE_ELEMENT* l_comparedValue = At_unchecked(p_genericArray, i);
				short int l_compareValue = p_elementSorter->Function(l_minValue, l_comparedValue, p_elementSorter->UserObject);
				if (l_compareValue >= 0)
				{
					l_minValue = l_comparedValue;
					l_minIndex = i;
				}
			}
		}
		return l_minIndex;
	};

	template <class DATA_CONTAINER, class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	void Sort_selection(DATA_CONTAINER* p_genericArray, ElementSorterT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT>* p_elementSorter)
	{
		for (size_t i = 0; i < p_genericArray->Size; i++)
		{
			size_t l_minIndex = MinIndex(p_genericArray, p_elementSorter, i);
			if (l_minIndex != i)
			{
				Swap(p_genericArray, i, l_minIndex);
			}
		}
	};

	template <class DATA_CONTAINER, class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	__forceinline void Sort_selection(DATA_CONTAINER* p_genericArray, ElementSorterT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT>&& p_elementSorter)
	{
		Sort_selection(p_genericArray, &p_elementSorter);
	};
}