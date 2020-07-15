#include "Equals.hpp"

#include <math.h>
#include "Constants.hpp"

namespace _Core
{

	bool Equals_ptr(void** p_left, void** p_right)
	{
		return *p_left == *p_right;
	};

	bool Equals_float(float* p_left, float* p_right)
	{
		return fabsf((*p_left) - (*p_right)) <= FLOAT_TOLERANCE;
	};
	
	bool Equals_int16t(int16_t* p_left, int16_t* p_right)
	{
		return *p_left == *p_right;
	};
	
	bool Equals_sizet(size_t* p_left, size_t* p_right)
	{
		return *p_left == *p_right;
	};
}