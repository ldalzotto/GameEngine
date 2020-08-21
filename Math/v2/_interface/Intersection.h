#pragma once

#include <stdbool.h>
#include "v2/_interface/SegmentC_def.h"
#include "v2/_interface/SphereC_def.h"
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/FrustumC_def.h"


bool Intersection_AABB_Ray(const BoxF_PTR p_AABB, const Segment_Vector3f_PTR p_ray, Vector3f_PTR p_outIntersectionPoint);
bool Intersection_AABB_Sphere(const BoxF_PTR p_AABB, const Spheref_PTR p_sphere);
bool Contains_AABB_Sphere(const BoxF_PTR p_AABB, const Spheref_PTR p_sphere);

// Sphere is projected in frustum space
bool Intersection_Contains_Frustum_Sphere(const Frustum_PTR p_frustum, const Spheref_PTR p_frustumProjectedSphere);