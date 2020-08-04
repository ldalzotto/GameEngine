#include "RSegment.h"

#include "v2/Vector/RVector.h"

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
}