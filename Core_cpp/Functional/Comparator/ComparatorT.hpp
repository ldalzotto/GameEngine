#pragma once

#include "Comaprator.hpp"

namespace _Core
{
	template <typename SOURCE_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	using ComparatorT_function = bool(*)(SOURCE_TYPE* p_sourceObject, COMPARED_TYPE* p_comparedObject, USER_TYPE* p_userObject);

	template <typename SOURCE_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	struct ComparatorT
	{
		ComparatorT_function<SOURCE_TYPE, COMPARED_TYPE, USER_TYPE> Function;
		COMPARED_TYPE* ComparedObject;
		USER_TYPE* UserObject;
	};

	template <typename SOURCE_TYPE, typename COMPARED_TYPE, typename USER_TYPE>
	struct AsserterT
	{
		ComparatorT_function<SOURCE_TYPE, COMPARED_TYPE, USER_TYPE> Function;
		USER_TYPE* UserObject;
	};



}