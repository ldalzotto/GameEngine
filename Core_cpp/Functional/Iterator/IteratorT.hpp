#pragma once

#include "Iterator.hpp"

namespace _Core
{
	template <typename ELEMENT_TYPE>
	struct VectorIteratorT
	{
		GenericArray* Array;
		ELEMENT_TYPE* Current;
		size_t CurrentIndex;
	};

	template <typename ELEMENT_TYPE>
	inline bool VectorIteratorT_moveToIndex(VectorIteratorT<ELEMENT_TYPE>* p_vectorIterator, size_t p_index)
	{
		return VectorIterator_moveToIndex((VectorIterator*)p_vectorIterator, p_index);
	};

	template <typename ELEMENT_TYPE>
	inline bool VectorIteratorT_moveNext(VectorIteratorT<ELEMENT_TYPE>* p_vectorIterator)
	{
		return VectorIterator_moveNext((VectorIterator*)p_vectorIterator);
	};

	template <typename ELEMENT_TYPE>
	struct VectorReverseIteratorT
	{
		GenericArray* Array;
		ELEMENT_TYPE* Current;
		size_t CurrentIndex;
	};

	template <typename ELEMENT_TYPE>
	inline bool VectorReverseIteratorT_moveNext(VectorReverseIteratorT<ELEMENT_TYPE>* p_vectorIterator)
	{
		return VectorReverseIterator_moveNext((VectorIterator*)p_vectorIterator);
	};

	template <typename ELEMENT_TYPE>
	struct VectorRangedIteratorT
	{
		GenericArray* Array;
		ELEMENT_TYPE* Current;
		size_t CurrentIndex;
		size_t Begin;
		size_t End;
	};

	template <typename ELEMENT_TYPE>
	inline bool VectorRangedIteratorT_moveNext(VectorRangedIteratorT<ELEMENT_TYPE>* p_vectorIterator)
	{
		return VectorRangedIterator_moveNext((VectorRangedIterator*)p_vectorIterator);
	};

}