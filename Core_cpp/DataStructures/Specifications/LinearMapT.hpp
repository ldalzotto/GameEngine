#pragma once

#include "DataStructures/GenericArray.hpp"
#include "Functional/Comparator/ComparatorT.hpp"

namespace _Core
{
	template <typename ELEMENT_TYPE>
	struct LinearMapT
	{
		ELEMENT_TYPE* Memory;
		size_t Size;
		size_t Capacity;
		size_t ElementSize;
	};


	template <typename ELEMENT_TYPE>
	inline void LinearMapT_alloc(LinearMapT<ELEMENT_TYPE>* p_linearMap, size_t p_initialCapacity)
	{
		GenericArray_alloc((GenericArray*)p_linearMap, sizeof(ELEMENT_TYPE), p_initialCapacity);
	}

	template <typename ELEMENT_TYPE>
	inline void LinearMapT_free(LinearMapT<ELEMENT_TYPE>* p_linearMap)
	{
		GenericArray_free((GenericArray*)p_linearMap);
	}

	template <typename ELEMENT_TYPE, typename USER_TYPE>
	inline void LinearMapT_pushBack(LinearMapT<ELEMENT_TYPE>* p_linearMap, ELEMENT_TYPE& p_element, AsserterT<ELEMENT_TYPE, ELEMENT_TYPE, USER_TYPE>& p_keyInsertionGuard)
	{
		GenericArray_pushBack_realloc_guarded((GenericArray*)p_linearMap, &p_element, (Asserter*)&p_keyInsertionGuard);
	}

}