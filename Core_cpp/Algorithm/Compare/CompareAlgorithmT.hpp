#pragma once

#include "CompareAlgorithm.hpp"

#include "Functional/Comparator/ComparatorT.hpp"
#include "Functional/Iterator/IteratorT.hpp"

namespace _Core
{
	struct VectorIterator;
	struct Comparator;
}

namespace _Core
{
	template <typename ELEMENT_TYPE>
	inline bool CompareT_find(VectorIteratorT<ELEMENT_TYPE>* p_iterator, ComparatorT<ELEMENT_TYPE, ELEMENT_TYPE, void>* p_comparator)
	{
		return Compare_find((VectorIterator*)p_iterator, (Comparator*)p_comparator);
	};
	
	template <typename ELEMENT_TYPE>
	bool CompareT_contains(VectorIteratorT<ELEMENT_TYPE>* p_iterator, ComparatorT<ELEMENT_TYPE, ELEMENT_TYPE, void>* p_comparator)
	{
		return CompareT_contains((VectorIterator*)p_iterator, (Comparator*)p_comparator);
	};
}