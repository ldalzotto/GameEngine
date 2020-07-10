#include "Equals.h"

#include <math.h>
#include "Constants.h"

bool CORE_EQUALS(float, float)(float* p_left, float* p_right)
{
	return fabsf((*p_left) - (*p_right)) <= FLOAT_TOLERANCE;
}