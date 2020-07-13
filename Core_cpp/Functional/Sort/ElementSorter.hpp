#pragma once

#include <stdint.h>

namespace _Core
{

	typedef short int(*ElementSort_function)(void* p_sourceObject, void* p_comparedObject, void* p_userObject);

	struct ElementSorter
	{
		ElementSort_function Function;
		void* UserObject;

	};

	short int SortCompare_sizet_sizet(size_t* p_left, size_t* p_right);
	short int SortCompare_float_float(float* p_left, float* p_right);
	short int SortCompare_uint16_uint16(uint16_t* p_left, uint16_t* p_right);
}