#include "Equals.hpp"

#include <math.h>
#include "Constants.hpp"

namespace _Core
{

	bool Core_Equals(float* p_left, float* p_right)
	{
		return fabsf((*p_left) - (*p_right)) <= FLOAT_TOLERANCE;
	};
	
	bool Core_Equals(int16_t* p_left, int16_t* p_right)
	{
		return *p_left == *p_right;
	};
	
	bool Core_Equals(size_t* p_left, size_t* p_right)
	{
		return *p_left == *p_right;
	};
}