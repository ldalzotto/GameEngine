#pragma once

#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Segment/SegmentV2.hpp"
extern "C"
{
#include "v2/_source/SegmentC.h"
}

namespace _MathV2
{
	struct SegmentM
	{
		template <typename T>
		inline static _MathV2::SegmentV2<4, T> cast(const _MathV2::SegmentV2<3, T>* p_segment, const T p_scalar)
		{
			_MathV2::SegmentV2<4, T> l_return;
			l_return.Begin = _MathV2::VectorM::cast(&p_segment->Begin);
			l_return.End = _MathV2::VectorM::cast(&p_segment->End);
			return l_return;
		};

		template <typename T>
		inline static _MathV2::SegmentV2<3, T> cast(const _MathV2::SegmentV2<4, T>* p_segment)
		{
			_MathV2::SegmentV2<3, T> l_return;
			l_return.Begin = *(Vector<3, T>*)&p_segment->Begin;
			l_return.End = *(Vector<3, T>*)&p_segment->End;
			return l_return;
		};
		
		inline static _MathV2::Vector<3, float>* direction(const _MathV2::SegmentV2<3, float>* p_segment, _MathV2::Vector<3, float>* p_out)
		{
			Seg_Direction_V3F((const SEGMENT_VECTOR3F_PTR)p_segment, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static _MathV2::Vector<4, float>* toVector(const _MathV2::SegmentV2<4, float>* p_segment, _MathV2::Vector<4, float>* p_out)
		{
			Seg_ToVector_V4F((const SEGMENT_VECTOR4F_PTR)p_segment, (VECTOR4F_PTR)p_out);
			return p_out;
		};

		inline static _MathV2::Vector<3, float>* toVector(const _MathV2::SegmentV2<3, float>* p_segment, _MathV2::Vector<3, float>* p_out)
		{
			Seg_ToVector_V3F((const SEGMENT_VECTOR3F_PTR)p_segment, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static _MathV2::SegmentV2<4, float>* mul(const _MathV2::SegmentV2<4, float>* p_segment, const _MathV2::Matrix<4, 4, float>* p_matrix, _MathV2::SegmentV2<4, float>* p_out)
		{
			Seg_Mul_V4F_M4F((const SEGMENT_VECTOR4F_PTR)p_segment, (const MATRIX4F_PTR)p_matrix, (SEGMENT_VECTOR4F_PTR)p_out);
			return p_out;
		};
	};
}
