#pragma once

#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	template <int C, int L, typename T>
	struct Matrix;
}

namespace _GameEngine::_Math
{
	struct Segment
	{
		_MathV2::Vector3<float> Begin;
		_MathV2::Vector3<float> End;
	};

	_MathV2::Vector3<float> Segment_direction(Segment* p_segment);
	_MathV2::Vector3<float> Segment_toVector(Segment* p_segment);
	_MathV2::Vector3<float> Segment_toVector(Segment& p_segment);
	float Segment_length(Segment* p_segment);
	void Segment_mul(Segment* p_segment, _MathV2::Matrix<4, 4, float>& p_matrix, Segment* p_out);
	void Segment_mul(Segment* p_segment, _MathV2::Matrix<3, 3, float>& p_matrix, Segment* p_out);
}