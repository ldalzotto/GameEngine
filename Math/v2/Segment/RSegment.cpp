/*
#include "RSegment.h"

#include "v2/Vector/RVector.h"
#include "v2/Matrix/RMatrix.h"

namespace _MathV2
{
	void RSegment_direction(const RSegment* p_segment, float* p_out)
	{
		RVector_min_specification((const float*)p_segment->End, (const float*)p_segment->Begin, p_out, p_segment->ElementNumber);
		RVector_normalize_specialization(p_out, p_segment->ElementNumber, p_out);
	};

	void RSegment_toVector(const RSegment* p_segment, float* p_out)
	{
		RVector_min_specification((const float*)p_segment->End, (const float*)p_segment->Begin, p_out, p_segment->ElementNumber);
	};

	float RSegment_length(const RSegment* p_segment, float* p_tempVec_0)
	{
		RSegment_toVector(p_segment, p_tempVec_0);
		return RVector_length_specialization(p_tempVec_0, p_segment->ElementNumber);
	};


	void RSegment_mul(const RSegment* p_segment, const RMatrix* p_matrix, float* p_tmp_vec, RSegment* p_out)
	{
		// RMatrix_3x3_mul_3x3
		//		_MathV2::Vector4<float> tmp_vec4;
		//		p_out->Begin = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_segment->Begin, 1.0f), &tmp_vec4));
		//		p_out->End = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_segment->End, 1.0f), &tmp_vec4));
		//		return p_out;
	};
}
*/