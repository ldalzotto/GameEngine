#pragma once

#include <math.h>

namespace _Math
{
	template <typename INPUT, typename OUTPUT>
	OUTPUT sqrt(INPUT p_value);

	template <>
	float sqrt<float, float>(float p_value)
	{
		return sqrtf(p_value);
	}
}