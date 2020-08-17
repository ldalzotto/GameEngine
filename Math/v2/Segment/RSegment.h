#pragma once

namespace _MathV2
{
	struct RSegment
	{
		short int ElementNumber;
		void* Begin;
		void* End;
	};

	void RSegment_direction(const RSegment* p_segment, float* p_out);
	void RSegment_toVector(const RSegment* p_segment, float* p_out);
}