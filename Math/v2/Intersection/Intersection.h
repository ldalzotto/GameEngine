#pragma once

namespace _MathV2
{
	struct Box;
	struct Segment;

	template <int N, typename T>
	struct Vector;
}

namespace _MathV2
{
	bool Intersection_AABB_Ray(const Box& p_AABB, const Segment& p_ray, _MathV2::Vector<3, float>* p_outIntersectionPoint);

}