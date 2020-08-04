#include "v2/Segment/SegmentV2Math.hpp"

using namespace _MathV2;

int main()
{
	_MathV2::SegmentV2<3, float> l_segment;
	l_segment.Begin = { 1.0f, 1.0f, 1.0f }; l_segment.End = { 3.0f, 3.0f, 3.0f };
	_MathV2::Vector3<float> l_dir; _MathV2::SegmentM::direction(&l_segment, &l_dir);
	_MathV2::Vector3<float> l_vec; _MathV2::SegmentM::toVector(&l_segment, &l_vec);
	_MathV2::SegmentV2<3, float> l_mul; _MathV2::SegmentM::mul(&l_segment, &Matrix3x3f_Identity, &l_mul);
	return 0;
}