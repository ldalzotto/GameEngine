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

	template <typename ELEMENT_TYPE>
	inline VectorIteratorT<ELEMENT_TYPE> SortedLinearMapT_buildIterator(SortedLinearMapT<ELEMENT_TYPE>* p_array)
	{
		VectorIteratorT<ELEMENT_TYPE> l_it;
		GenericArray_buildIterator((GenericArray*)p_array, (VectorIterator*)&l_it);
		return l_it;
	}

	template <typename ELEMENT_TYPE, 
				typename GUARD_COMPARE_TYPE, typename GUARD_USER_TYPE, 
				typename SORT_COMPARE_TYPE, typename SORT_USER_TYPE>
	inline ELEMENT_TYPE* SortedLinearMapT_pushBack(SortedLinearMapT<ELEMENT_TYPE>* p_linearMap, ELEMENT_TYPE& p_element,
			AsserterT<ELEMENT_TYPE, GUARD_COMPARE_TYPE, GUARD_USER_TYPE>& p_keyInsertionGuard,
			ElementSorterT<ELEMENT_TYPE, SORT_COMPARE_TYPE, SORT_USER_TYPE>& p_elementSorter)
	{
		return (ELEMENT_TYPE*)GenericArray_pushBack_realloc_guarded_sorted((GenericArray*)p_linearMap, &p_element, (Asserter*)&p_keyInsertionGuard, (ElementSorter*)&p_elementSorter);
	}

	template <typename ELEMENT_TYPE>
	inline void SortedLinearMapT_erase(SortedLinearMapT<ELEMENT_TYPE>* p_array, size_t p_index)
	{
		GenericArray_erase((GenericArray*)p_array, p_index);
	};

	template <typename ELEMENT_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	inline void SortedLinearMapT_eraseCompare(SortedLinearMapT<ELEMENT_TYPE>* p_array, ComparatorT<ELEMENT_TYPE, COMPARED_TYPE, USER_TYPE>&& p_comparator)
	{
		GenericArray_eraseCompare((GenericArray*)p_array, (Comparator*)&p_comparator);
	};


	template <typename ELEMENT_TYPE>
	inline SortedLinearMapT<ELEMENT_TYPE> SortedLinearMapT_deepCopy(SortedLinearMapT<ELEMENT_TYPE>* p_array)
	{
		GenericArray l_arr = GenericArray_deepCopy((GenericArray*)p_array);
		return *(SortedLinearMapT<ELEMENT_TYPE>*) & l_arr;
	};

}