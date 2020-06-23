#pragma once

namespace _GameEngine::_Math
{
	# define M_PI            3.14159265358979323846f
	# define FLOAT_TOLERANCE 0.000001f

	extern float DEG_TO_RAD;
	extern float RAD_TO_DEG;

	double Math_max(double left, double right);
	double Math_min(double left, double right);
}