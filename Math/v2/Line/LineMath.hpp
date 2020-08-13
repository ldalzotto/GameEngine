#pragma once

#include "Line.hpp"

namespace _MathV2
{
	struct Line2DEquation
	{
		float A, B;
		bool IsVertical;
		// Vector<2, f> Point;
	};

	struct LineM
	{
		static Line2DEquation equation(Line2D* p_line);
		static float getX(Line2DEquation* p_eq, float p_y);
		static float getY(Line2DEquation* p_eq, float p_x);
	};
}