#pragma once

namespace _GameEngine::_Math
{
	struct Box;
	struct Plane;
	struct Segment;
	struct Vector3f;
}

namespace _GameEngine::_Math
{
	bool Intersection_AABB_Ray(Box* p_AABB, Segment* p_ray, Vector3f* p_outIntersectionPoint);

}