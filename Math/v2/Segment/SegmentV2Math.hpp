#pragma once

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Segment/SegmentV2.hpp"
#include "v2/Segment/RSegment.h"

namespace _MathV2
{
	struct SegmentM
	{
		template <int N, typename T>
		static _MathV2::Vector<N, T>* direction(const _MathV2::SegmentV2<N, T>* p_segment, _MathV2::Vector<N, T>* p_out)
		{
			RSegment l_rSegment = {N, (void*)&p_segment->Begin, (void*)&p_segment->End};
			RSegment_direction(&l_rSegment, (T*)p_out);
			return p_out;
		};

		template <int N, typename T>
		static _MathV2::Vector<N, T>* toVector(const _MathV2::SegmentV2<N, T>* p_segment, _MathV2::Vector<N, T>* p_out)
		{
			RSegment l_rSegment = { N, (void*)&p_segment->Begin, (void*)&p_segment->End };
			RSegment_toVector(&l_rSegment, (T*)p_out);
			return p_out;
		};

		template <int N, typename T>
		static _MathV2::SegmentV2<N, T>* mul(const _MathV2::SegmentV2<N, T>* p_segment, const _MathV2::Matrix<N, N, T>* p_matrix, _MathV2::SegmentV2<N, T>* p_out)
		{
			MatrixM::mul(p_matrix, &p_segment->Begin, &p_out->Begin);
			MatrixM::mul(p_matrix, &p_segment->End, &p_out->End);
			return p_out;
		};
	};
}