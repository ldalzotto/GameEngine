#include "SegmentMath.hpp"

#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

using namespace _MathV2;

namespace _GameEngine::_Math
{

	_MathV2::Vector3<float> SegmentM::direction(const Segment* p_segment)
	{
		return VectorM::normalize(VectorM::min(p_segment->End, p_segment->Begin));
	};

	_MathV2::Vector3<float> SegmentM::toVector(const Segment& p_segment)
	{
		return VectorM::min(p_segment.End, p_segment.Begin);
	};

	float Segment_length(const Segment& p_segment)
	{
		return VectorM::length(SegmentM::toVector(p_segment));
	};

	Segment SegmentM::mul(const Segment& p_segment, const _MathV2::Matrix<4, 4, float>& p_matrix)
	{
		return Segment{
			VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_segment.Begin, 1.0f))),
			VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_segment.End, 1.0f)))
		};
	};

	Segment SegmentM::mul(const Segment& p_segment, const _MathV2::Matrix<3, 3, float>& p_matrix)
	{
		return Segment
		{
			MatrixM::mul(p_matrix, p_segment.Begin),
			MatrixM::mul(p_matrix, p_segment.End)
		};
	};
}