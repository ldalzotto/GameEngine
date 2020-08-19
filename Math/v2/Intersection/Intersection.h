#pragma once

extern "C"
{
#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/SphereC_def.h"
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/FrustumC_def.h"
}

namespace _MathV2
{
	bool Intersection_AABB_Ray(const BOXF_PTR p_AABB, const SEGMENT_VECTOR3F_PTR p_ray, VECTOR3F_PTR p_outIntersectionPoint);
	bool Intersection_AABB_Sphere(const BOXF_PTR p_AABB, const SPHEREF_PTR p_sphere);
	bool Contains_AABB_Sphere(const BOXF_PTR p_AABB, const SPHEREF_PTR p_sphere);

	// Sphere is projected in frustum space
	bool Intersection_Contains_Frustum_Sphere(const FRUSTUM_PTR p_frustum, const SPHEREF_PTR p_frustumProjectedSphere);
}