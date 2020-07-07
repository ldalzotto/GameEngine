#pragma message(__FILE__)

namespace _CoreV3
{


	template<class DATA_STRUCTURE, class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	SOURCE_ELEMENT* Find(DATA_STRUCTURE* p_array, ElementComparatorT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT>* p_comparator)
	{
		SOURCE_ELEMENT* l_currentElement = NULL;
		for (size_t i = 0; i < p_array->Size; i++)
		{
			l_currentElement = At(p_array, i);
			if (p_comparator->Function(l_currentElement, p_comparator->Right, p_comparator->UserObject))
			{
				return l_currentElement;
			}
		}

		return NULL;
	};

	template<class DATA_STRUCTURE, class SOURCE_ELEMENT, class COMPARED_ELEMENT, class USER_OBJECT>
	__forceinline SOURCE_ELEMENT* Find(DATA_STRUCTURE* p_array, ElementComparatorT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT>&& p_comparator)
	{
		return Find(p_array, (ElementComparatorT<SOURCE_ELEMENT, COMPARED_ELEMENT, USER_OBJECT>*) & p_comparator);
	};
}
