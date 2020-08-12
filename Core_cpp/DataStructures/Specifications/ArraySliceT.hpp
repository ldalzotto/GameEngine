#pragma once

#include "DataStructures/GenericArray.hpp"
#include "Functional/Iterator/IteratorT.hpp"

namespace _Core
{
	template<typename ELEMENT_TYPE>
	struct ArraySliceT
	{
		GenericArray* Array;
		size_t BeginInlcuded;
		size_t EndExcluded;
	};

	template<typename ELEMENT_TYPE>
	VectorRangedIteratorT<ELEMENT_TYPE> ArraySliceT_buildIterator(const ArraySliceT<ELEMENT_TYPE>* p_array)
	{
		VectorRangedIteratorT<ELEMENT_TYPE> l_it;
		GenericArray_buildRangedIterator(p_array->Array, (VectorRangedIterator*)&l_it, p_array->BeginInlcuded, p_array->EndExcluded);
		return l_it;
	}
}