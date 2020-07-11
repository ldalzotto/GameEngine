#pragma once

#include <stdlib.h>

typedef short int(*Core_elementSort_function)(void* p_sourceObject, void* p_comparedObject, void* p_userObject);

typedef struct Core_ElementSorter
{
	Core_elementSort_function Function;
	void* UserObject;

} Core_ElementSorter;

short int Core_sortCompare_sizet_sizet(size_t* p_left, size_t* p_right);
short int Core_sortCompare_float_float(float* p_left, float* p_right);
short int Core_sortCompare_uint16_uint16(unsigned __int16* p_left, unsigned __int16* p_right);