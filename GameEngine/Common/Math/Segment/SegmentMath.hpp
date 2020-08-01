#pragma once

#include "Segment.hpp"

namespace _MathV2
{
	template <int C, int L, typename T>
	struct Matrix;
}

namespace _GameEngine::_Math
{
	struct SegmentM
	{
		static _MathV2::Vector3<float> direction(const Segment* p_segment);
		static _MathV2::Vector3<float> toVector(const Segment& p_segment);
		static float length(const Segment& p_segment);
		static Segment mul(const Segment& p_segment, const _MathV2::Matrix<4, 4, float>& p_matrix);
		static Segment mul(const Segment& p_segment, const _MathV2::Matrix<3, 3, float>& p_matrix);
	};
}