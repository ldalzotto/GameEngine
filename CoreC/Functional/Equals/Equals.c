#include "Equals.h"

#include <math.h>
#include "Constants.h"

bool CORE_EQUALS(float, float)(float* p_left, float* p_right)
{
	return fabsf((*p_left) - (*p_right)) <= FLOAT_TOLERANCE;
}

bool CORE_EQUALS(uint16, uint16)(unsigned __int16* p_left, unsigned __int16* p_right)
{
	return *p_left == *p_right;
};

bool CORE_EQUALS(sizet, sizet)(size_t* p_left, size_t* p_right)
{
	return *p_left == *p_right;
};