#include "LineMath.hpp"

#include <math.h>
#include "Constants.hpp"

namespace _MathV2
{
	Line2DEquation LineM::equation(Line2D* p_line)
	{
		Line2DEquation l_eq;
		l_eq.IsVertical = fabsf(p_line->End.x - p_line->Begin.x) <= FLOAT_TOLERANCE;
		if (!l_eq.IsVertical)
		{
			l_eq.A = (p_line->End.y - p_line->Begin.y) / (p_line->End.x - p_line->Begin.x);
		}
		else
		{
			l_eq.A = p_line->End.x;
		}
		l_eq.B = p_line->Begin.y - (l_eq.A * p_line->Begin.x);
		return l_eq;
	}
	
	float LineM::getX(Line2DEquation* p_eq, float p_y)
	{
		if (p_eq->IsVertical)
		{
			return p_eq->A;
		}
		else
		{
			return ((p_y - p_eq->B) / p_eq->A);
		}
	};

	float LineM::getY(Line2DEquation* p_eq, float p_x)
	{
		if (p_eq->IsVertical)
		{
			return 0.0f;
		}
		else
		{
			return (p_eq->A * p_x) + p_eq->B;
		}
	};
}