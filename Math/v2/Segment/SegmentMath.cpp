#include "SegmentMath.hpp"

#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

namespace _MathV2
{

	_MathV2::Vector3<float>* SegmentM::direction(const Segment* p_segment, _MathV2::Vector3<float>* p_out)
	{
		VectorM::normalize(VectorM::min(&p_segment->End, &p_segment->Begin, p_out), p_out);
		return p_out;
	};

	_MathV2::Vector3<float>* SegmentM::toVector(const Segment* p_segment, _MathV2::Vector3<float>* p_out)
	{
		VectorM::min(&p_segment->End, &p_segment->Begin, p_out);
		return p_out;
	};

	float Segment_length(const Segment* p_segment)
	{
		_MathV2::Vector3<float> tmp_vec3;
		return VectorM::length(SegmentM::toVector(p_segment, &tmp_vec3));
	};

	Segment* SegmentM::mul(const Segment* p_segment, const _MathV2::Matrix<4, 4, float>* p_matrix, Segment* p_out)
	{
		_MathV2::Vector4<float> tmp_vec4;
		p_out->Begin = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_segment->Begin, 1.0f), &tmp_vec4));
		p_out->End = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_segment->End, 1.0f), &tmp_vec4));
		return p_out;
	};
}