#pragma once

#include <math.h>

namespace _MathV2
{
	# define M_PI            3.14159265358979323846f

	extern float DEG_TO_RAD;
	extern float RAD_TO_DEG;

	double Math_max(double left, double right);
	double Math_min(double left, double right);

	template<typename T>
	T Math_sqrt(T p_value);
}