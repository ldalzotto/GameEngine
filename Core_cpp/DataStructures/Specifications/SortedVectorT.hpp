#pragma once

#include "DataStructures/GenericArray.hpp"
#include "Functional/Sort/ElementSorterT.hpp"

namespace _Core
{
	template <typename ELEMENT_TYPE>
	struct SortedVectorT
	{
		ELEMENT_TYPE* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	template <typename ELEMENT_TYPE>
	inline void SortedVectorT_alloc(SortedVectorT<ELEMENT_TYPE>* p_sortedVector, size_t p_initialCapcity)
	{
		GenericArray_alloc((GenericArray*)p_sortedVector, sizeof(ELEMENT_TYPE), p_initialCapcity);
	};

	template <typename ELEMENT_TYPE>
	inline void SortedVectorT_free(SortedVectorT<ELEMENT_TYPE>* p_sortedVector)
	{
		GenericArray_free((GenericArray*)p_sortedVector);
	};

	template <typename ELEMENT_TYPE>
	inline void SortedVectorT_pushBack(SortedVectorT<ELEMENT_TYPE>* p_sortedVector, ELEMENT_TYPE* p_element, ElementSortT_function<ELEMENT_TYPE, ELEMENT_TYPE, void> p_sortFunction)
	{
		ElementSorter l_sorter;
		l_sorter.Function = (ElementSort_function)p_sortFunction;
		GenericArray_pushBack_realloc_sorted((GenericArray*)p_sortedVector, p_element, &l_sorter);
	};

	template <typename ELEMENT_TYPE>
	inline VectorIteratorT<ELEMENT_TYPE> SortedVectorT_buildIterator(SortedVectorT<ELEMENT_TYPE>* p_array)
	{
		VectorIteratorT<ELEMENT_TYPE> l_it;
		GenericArray_buildIterator((GenericArray*)p_array, (VectorIterator*)&l_it);
		return l_it;
	}

	template <typename ELEMENT_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	inline void SortedVectorT_eraseCompare(SortedVectorT<ELEMENT_TYPE>* p_sortedVector, ComparatorT<ELEMENT_TYPE, COMPARED_TYPE, USER_TYPE>&& p_comparator)
	{
		GenericArray_eraseCompare((GenericArray*)p_sortedVector, (Comparator*)&p_comparator);
	};
}