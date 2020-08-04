#pragma once

#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	template <int N, typename T>
	struct SegmentV2
	{
		_MathV2::Vector<N, T> Begin;
		_MathV2::Vector<N, T> End;
	};
}