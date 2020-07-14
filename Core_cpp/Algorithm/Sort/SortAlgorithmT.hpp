#pragma once

#include <functional>

#include "Functional/Sort/ElementSorterT.hpp"
#include "Functional/Iterator/IteratorT.hpp"
#include "SortAlgorithm.hpp"

namespace _Core
{
	template <typename ELEMENT_TYPE, typename COMPERED_TYPE, typename USER_TYPE>
	inline void SortT_min(VectorIteratorT<ELEMENT_TYPE>* p_iterator, size_t p_startIndex, ElementSorterT<ELEMENT_TYPE, COMPERED_TYPE, USER_TYPE>* p_sortComparator)
	{
		Sort_min((VectorIterator*)p_iterator, p_startIndex, (ElementSorter*)p_sortComparator);
	};
	
	template <typename ELEMENT_TYPE>
	inline VectorIteratorT<ELEMENT_TYPE>&& SortT_selectionSort(VectorIteratorT<ELEMENT_TYPE>& p_iterator, ElementSorterT<ELEMENT_TYPE, ELEMENT_TYPE, void>& p_sortComparator)
	{
		Sort_selectionSort((VectorIterator*)&p_iterator, (ElementSorter*)&p_sortComparator);
		return std::move(p_iterator);
	};
}