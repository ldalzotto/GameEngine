#pragma once

#include "DataStructures/GenericArray.hpp"
#include "Functional/Iterator/IteratorT.hpp"

namespace _Core
{
	template <typename ELEMENT_TYPE>
	struct ArrayT
	{
		ELEMENT_TYPE* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};

	template <typename ELEMENT_TYPE>
	inline void ArrayT_alloc(ArrayT<ELEMENT_TYPE>* p_array, size_t p_initialCapacity)
	{
		GenericArray_alloc((GenericArray*)p_array, sizeof(ELEMENT_TYPE), p_initialCapacity);
	}

	template <typename ELEMENT_TYPE>
	inline void ArrayT_free(ArrayT<ELEMENT_TYPE>* p_array)
	{
		GenericArray_free((GenericArray*)p_array);
	}

	template <typename ELEMENT_TYPE>
	inline void ArrayT_zeroing(ArrayT<ELEMENT_TYPE>* p_array)
	{
		GenericArray_zeroing((GenericArray*)p_array);
	}

	template <typename ELEMENT_TYPE>
	inline ELEMENT_TYPE* ArrayT_at(ArrayT<ELEMENT_TYPE>* p_array, size_t p_index)
	{
		return (ELEMENT_TYPE*) GenericArray_at((GenericArray*)p_array, p_index);
	}

	template <typename ELEMENT_TYPE>
	inline void ArrayT_pushBack(ArrayT<ELEMENT_TYPE>* p_array, ELEMENT_TYPE* p_element)
	{
		GenericArray_pushBack_noRealloc((GenericArray*)p_array, p_element);
	}

	template <typename ELEMENT_TYPE>
	inline void ArrayT_pushBack(ArrayT<ELEMENT_TYPE>* p_array, ELEMENT_TYPE& p_element)
	{
		GenericArray_pushBack_noRealloc((GenericArray*)p_array, &p_element);
	}

	template <typename ELEMENT_TYPE>
	inline void ArrayT_pushBack(ArrayT<ELEMENT_TYPE>* p_array, ELEMENT_TYPE&& p_element)
	{
		GenericArray_pushBack_noRealloc((GenericArray*)p_array, &p_element);
	}

	template <typename ELEMENT_TYPE>
	inline void ArrayT_clear(ArrayT<ELEMENT_TYPE>* p_array)
	{
		GenericArray_clear((GenericArray*)p_array);
	}

	template <typename ELEMENT_TYPE>
	inline VectorIteratorT<ELEMENT_TYPE> ArrayT_buildIterator(ArrayT<ELEMENT_TYPE>* p_array)
	{
		VectorIteratorT<ELEMENT_TYPE> l_it;
		GenericArray_buildIterator((GenericArray*)p_array, (VectorIterator*)&l_it);
		return l_it;
	}

	template <typename ELEMENT_TYPE>
	inline ArrayT<ELEMENT_TYPE> ArrayT_fromCStyleArray(ELEMENT_TYPE* p_array, size_t p_size)
	{
		ArrayT<ELEMENT_TYPE> l_arr =
		{
			p_array,
			p_size,
			p_size,
			sizeof(ELEMENT_TYPE)
		};
		return l_arr;
	}
}