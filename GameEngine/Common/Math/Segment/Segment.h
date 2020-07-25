#pragma once

#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{
	struct Matrix4x4f;
	struct Matrix3x3f;
}

namespace _GameEngine::_Math
{
	struct Segment
	{
		_Math::Vector3f Begin;
		_Math::Vector3f End;
	};

	_Math::Vector3f Segment_direction(Segment* p_segment);
	_Math::Vector3f Segment_toVector(Segment* p_segment);
	float Segment_length(Segment* p_segment);
	void Segment_mul(Segment* p_segment, _Math::Matrix4x4f* p_matrix, Segment* p_out);
	void Segment_mul(Segment* p_segment, _Math::Matrix3x3f* p_matrix, Segment* p_out);
}