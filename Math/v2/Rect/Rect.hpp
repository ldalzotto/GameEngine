#pragma once

#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	template <typename T>
	struct Rect
	{
		Vector<2, T> Min;
		Vector<2, T> Max;
	};
}