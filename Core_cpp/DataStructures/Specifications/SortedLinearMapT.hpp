#pragma once

#include "DataStructures/GenericArray.hpp"
#include "Functional/Comparator/ComparatorT.hpp"
#include "Functional/Sort/ElementSorterT.hpp"

namespace _Core
{
	template <typename ELEMENT_TYPE>
	struct SortedLinearMapT
	{
		ELEMENT_TYPE* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};


	template <typename ELEMENT_TYPE>
	inline void SortedLinearMapT_alloc(SortedLinearMapT<ELEMENT_TYPE>* p_linearMap, size_t p_initialCapacity)
	{
		GenericArray_alloc((GenericArray*)p_linearMap, sizeof(ELEMENT_TYPE), p_initialCapacity);
	}

	template <typename ELEMENT_TYPE>
	inline void SortedLinearMapT_free(SortedLinearMapT<ELEMENT_TYPE>* p_linearMap)
	{
		GenericArray_free((GenericArray*)p_linearMap);
	}

	template <typename ELEMENT_TYPE, typename GUARD_USER_TYPE, typename SORT_USER_TYPE>
	inline void SortedLinearMapT_pushBack(SortedLinearMapT<ELEMENT_TYPE>* p_linearMap, ELEMENT_TYPE& p_element, 
			AsserterT<ELEMENT_TYPE, ELEMENT_TYPE, GUARD_USER_TYPE>& p_keyInsertionGuard,
			ElementSorterT<ELEMENT_TYPE, ELEMENT_TYPE, SORT_USER_TYPE>& p_elementSorter)
	{
		GenericArray_pushBack_realloc_guarded_sorted((GenericArray*)p_linearMap, &p_element, (Asserter*)&p_keyInsertionGuard, (ElementSorter*)&p_elementSorter);
	}

}