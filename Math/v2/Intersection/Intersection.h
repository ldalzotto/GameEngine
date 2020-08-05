#pragma once

namespace _MathV2
{
	struct Box;

	template <int N, typename T>
	struct SegmentV2;

	template <int N, typename T>
	struct Vector;
}

namespace _MathV2
{
	bool Intersection_AABB_Ray(const Box* p_AABB, const SegmentV2<3, float>* p_ray, _MathV2::Vector<3, float>* p_outIntersectionPoint);

}