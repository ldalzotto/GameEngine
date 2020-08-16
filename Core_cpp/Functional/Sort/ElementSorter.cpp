#include "ElementSorter.hpp"

#include "Constants.h"

namespace _Core
{
#define INT_SORT_COMPARE(Left, Right) \
	if  (*Left > *Right) { return 1; } \
	else if (*Left < *Right ) { return -1; } \
	else { return 0; }

#define NUMERIC_SORT_COMPARE(Left, Right) \
	if  ((*Left - *Right) >= FLOAT_TOLERANCE) { return 1; } \
	else if ( (*Left -*Right) <= -1.0f * FLOAT_TOLERANCE ) { return -1; } \
	else { return 0; }

	short int SortCompare_sizet_sizet(size_t* p_left, size_t* p_right)
	{
		INT_SORT_COMPARE(p_left, p_right);
	};
	
	short int SortCompare_float_float(float* p_left, float* p_right)
	{
		NUMERIC_SORT_COMPARE(p_left, p_right);
	};

	short int SortCompare_float_float(float p_left, float p_right)
	{
		NUMERIC_SORT_COMPARE(&p_left, &p_right);
	};
	
	short int SortCompare_uint16_uint16(uint16_t* p_left, uint16_t* p_right)
	{
		INT_SORT_COMPARE(p_left, p_right);
	};
}