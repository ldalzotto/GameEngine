#pragma once

namespace _GameEngine::_Math
{
	struct Box;
	struct Plane;
	struct Segment;
}

namespace _MathV2
{
	template <int N, typename T>
	struct Vector;
}

namespace _GameEngine::_Math
{
	bool Intersection_AABB_Ray(const Box& p_AABB, const Segment& p_ray, _MathV2::Vector<3, float>* p_outIntersectionPoint);

}