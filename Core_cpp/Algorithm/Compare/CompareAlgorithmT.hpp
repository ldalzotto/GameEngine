#pragma once

#include "CompareAlgorithm.hpp"

#include <functional>

#include "Functional/Comparator/ComparatorT.hpp"
#include "Functional/Iterator/IteratorT.hpp"

namespace _Core
{
	struct VectorIterator;
	struct Comparator;
}

namespace _Core
{

	template <typename ELEMENT_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	inline VectorIteratorT<ELEMENT_TYPE>&& CompareT_find(VectorIteratorT<ELEMENT_TYPE>& p_iterator, const ComparatorT<ELEMENT_TYPE, COMPARED_TYPE, USER_TYPE>& p_comparator)
	{
		Compare_find((VectorIterator*)&p_iterator, (Comparator*)&p_comparator);
		return std::move(p_iterator);
	};

	
	template <typename ELEMENT_TYPE>
	bool CompareT_contains(VectorIteratorT<ELEMENT_TYPE>& p_iterator, ComparatorT<ELEMENT_TYPE, ELEMENT_TYPE, void>& p_comparator)
	{
		return CompareT_contains((VectorIterator*)&p_iterator, (Comparator*)&p_comparator);
	};
}