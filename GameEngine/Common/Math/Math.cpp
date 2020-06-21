#include "Math.h"

namespace _GameEngine::_Math
{
	float DEG_TO_RAD = (M_PI / 180.0f);
	float RAD_TO_DEG = (180.0f / M_PI);

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
}