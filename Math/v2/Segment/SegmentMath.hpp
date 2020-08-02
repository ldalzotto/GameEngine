#pragma once

#include "Segment.hpp"

namespace _MathV2
{
	template <int C, int L, typename T>
	struct Matrix;
}

namespace _MathV2
{
	struct SegmentM
	{
		static _MathV2::Vector3<float>* direction(const Segment* p_segment, _MathV2::Vector3<float>* p_out);
		static _MathV2::Vector3<float>* toVector(const Segment* p_segment, _MathV2::Vector3<float>* p_out);
		static float length(const Segment* p_segment);
		static Segment* SegmentM::mul(const Segment* p_segment, const _MathV2::Matrix<4, 4, float>* p_matrix, Segment* p_out);
	};
}