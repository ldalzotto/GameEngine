#pragma once

extern "C"
{
#include "v2/_interface/SegmentC_def.h"
}

namespace _MathV2
{
	struct Box;
	struct Sphere;

	template <int N, typename T>
	struct Vector;

	struct Frustum;
}

namespace _MathV2
{
	bool Intersection_AABB_Ray(const Box* p_AABB, const SEGMENT_VECTOR3F_PTR p_ray, _MathV2::Vector<3, float>* p_outIntersectionPoint);
	bool Intersection_AABB_Sphere(const Box* p_AABB, const Sphere* p_sphere);
	bool Contains_AABB_Sphere(const Box* p_AABB, const Sphere* p_sphere);

	// Sphere is projected in frustum space
	bool Intersection_Contains_Frustum_Sphere(const Frustum* p_frustum, const Sphere* p_frustumProjectedSphere);
}