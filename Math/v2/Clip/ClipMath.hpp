#pragma once

#include "v2/Rect/Rect.hpp"
#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	struct ClipM
	{
		static bool clip(const Vector<2, float>* in_clippedSegment_begin, const Vector<2, float>* in_clippedSegment_end,
			const Rect<int>* p_clippedRect, Vector<2, int>* out_clippedSegment_begin, Vector<2, int>* out_clippedSegment_end);
	};
}