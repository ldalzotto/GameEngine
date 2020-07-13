#pragma once

#include "ElementSorter.hpp"

namespace _Core
{
	template <typename SOURCE_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	using ElementSortT_function = short int(*)(SOURCE_TYPE* p_sourceObject, COMPARED_TYPE* p_comparedObject, USER_TYPE* p_userObject);

	template <typename SOURCE_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	struct ElementSorterT
	{
		ElementSortT_function<SOURCE_TYPE, COMPARED_TYPE, USER_TYPE> Function;
		USER_TYPE* UserObject;
	};

}