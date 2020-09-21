#pragma once

#include "v2/Math.h"

inline float _i_Math_clamp01f(float p_value)
{
	if (p_value <= 0.0f) { return 0.0f; }
	else if (p_value >= 1.0f) { return 1.0f; }
	else { return p_value; };
};

inline float _i_Math_clamp1f(float p_value)
{
	return (p_value >= 1.0f ? 1.0f : p_value);
};