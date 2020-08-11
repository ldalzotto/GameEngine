#include "Math.h"

namespace _MathV2
{
	float DEG_TO_RAD = (M_PI / 180.0f);
	float RAD_TO_DEG = (180.0f / M_PI);

#define MATH_MIN_NUMERICAL(Left, Right)

	double Math_max(double left, double right)
	{
		if (right > left)
		{
			return right;
		}
		else
		{
			return left;
		}
	};

	double Math_min(double left, double right)
	{
		if (right < left)
		{
			return right;
		}
		else
		{
			return left;
		}
	};

	template<>
	float Math_sqrt<float>(float p_value) { return sqrtf(p_value); };

	template<>
	float Math_sqrt<double>(double p_value) { return sqrt(p_value); };

	template<>
	float Math_sqrt<int>(int p_value) { return sqrt(p_value); };
}