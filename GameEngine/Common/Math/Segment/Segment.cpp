#include "Segment.h"

#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

using namespace _MathV2;

namespace _GameEngine::_Math
{

	_MathV2::Vector3<float> Segment_direction(Segment* p_segment)
	{
		return VectorM::normalize(VectorM::min(p_segment->End, p_segment->Begin));
	};

	_MathV2::Vector3<float> Segment_toVector(Segment* p_segment)
	{
		return VectorM::min(p_segment->End, p_segment->Begin);
	};

	_MathV2::Vector3<float> Segment_toVector(Segment& p_segment)
	{
		return Segment_toVector(&p_segment);
	};

	float Segment_length(Segment* p_segment)
	{
		return VectorM::length(Segment_toVector(p_segment));
	};

	void Segment_mul(Segment* p_segment, _MathV2::Matrix<4, 4, float>& p_matrix, Segment* p_out)
	{
		p_out->Begin = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_segment->Begin, 1.0f)));
		p_out->End = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_segment->End, 1.0f)));
	};

	void Segment_mul(Segment* p_segment, _MathV2::Matrix<3, 3, float>& p_matrix, Segment* p_out)
	{
		p_out->Begin = MatrixM::mul(p_matrix, p_segment->Begin);
		p_out->Begin = MatrixM::mul(p_matrix, p_segment->End);
	};
}