#include "v2/_interface/Intersection.h"

#include <math.h>

#include "Constants.h"
#include "Functional/Compare.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/SegmentC.h"
#include "v2/_interface/BoxC.h"
#include "v2/_interface/FrustumC.h"
#include "v2/_interface/RectC.h"

bool Intersection_AABB_Ray(const BoxF_PTR p_AABB, const Segment_Vector3f_PTR p_ray, Vector3f_PTR p_outIntersectionPoint)
{
	Vector3f l_boxMin, l_boxMax;
	Box_ExtractMinMax_F(p_AABB, &l_boxMin, &l_boxMax);

	/*
		Ray distance fractions are normalized % of length of the p_ray length.
	*/

	float l_rayDistanceFraction_min, l_rayDistanceFraction_max;

	float l_rayDistanceFractionX_min, l_rayDistanceFractionX_max;
	{
		float l_xMax = l_boxMax.x;
		float l_xMin = l_boxMin.x;

		l_rayDistanceFractionX_min = (l_xMin - p_ray->Begin.x) / (p_ray->End.x - p_ray->Begin.x);
		l_rayDistanceFractionX_max = (l_xMax - p_ray->Begin.x) / (p_ray->End.x - p_ray->Begin.x);
	}

	l_rayDistanceFraction_min = l_rayDistanceFractionX_min;
	l_rayDistanceFraction_max = l_rayDistanceFractionX_max;

	if (l_rayDistanceFraction_min > l_rayDistanceFraction_max)
	{
		float l_tmp = l_rayDistanceFraction_max;
		l_rayDistanceFraction_max = l_rayDistanceFraction_min;
		l_rayDistanceFraction_min = l_tmp;
	}

	float l_rayDistanceFractionY_min, l_rayDistanceFractionY_max;
	{
		float l_yMax = l_boxMax.y;
		float l_yMin = l_boxMin.y;

		l_rayDistanceFractionY_min = (l_yMin - p_ray->Begin.y) / (p_ray->End.y - p_ray->Begin.y);
		l_rayDistanceFractionY_max = (l_yMax - p_ray->Begin.y) / (p_ray->End.y - p_ray->Begin.y);
	}

	if (l_rayDistanceFractionY_min > l_rayDistanceFractionY_max)
	{
		float l_tmp = l_rayDistanceFractionY_max;
		l_rayDistanceFractionY_max = l_rayDistanceFractionY_min;
		l_rayDistanceFractionY_min = l_tmp;
	}

	if ((l_rayDistanceFraction_min > l_rayDistanceFractionY_max) || (l_rayDistanceFractionY_min > l_rayDistanceFraction_max))
	{
		return false;
	}

	if (l_rayDistanceFractionY_min > l_rayDistanceFraction_min)
	{
		l_rayDistanceFraction_min = l_rayDistanceFractionY_min;
	}
	if (l_rayDistanceFractionY_max < l_rayDistanceFraction_max)
	{
		l_rayDistanceFraction_max = l_rayDistanceFractionY_max;
	}

	float l_rayDistanceFractionZ_min, l_rayDistanceFractionZ_max;
	{
		float l_zMax = l_boxMax.z;
		float l_zMin = l_boxMin.z;

		l_rayDistanceFractionZ_min = (l_zMin - p_ray->Begin.z) / (p_ray->End.z - p_ray->Begin.z);
		l_rayDistanceFractionZ_max = (l_zMax - p_ray->Begin.z) / (p_ray->End.z - p_ray->Begin.z);
	}

	if (l_rayDistanceFractionZ_min > l_rayDistanceFractionZ_max)
	{
		float l_tmp = l_rayDistanceFractionZ_max;
		l_rayDistanceFractionZ_max = l_rayDistanceFractionZ_min;
		l_rayDistanceFractionZ_min = l_tmp;
	}

	if ((l_rayDistanceFraction_min > l_rayDistanceFractionZ_max) || (l_rayDistanceFractionZ_min > l_rayDistanceFraction_max))
	{
		return false;
	}

	if (l_rayDistanceFractionZ_min > l_rayDistanceFraction_min)
	{
		l_rayDistanceFraction_min = l_rayDistanceFractionZ_min;
	}
	if (l_rayDistanceFractionZ_max < l_rayDistanceFraction_max)
	{
		l_rayDistanceFraction_max = l_rayDistanceFractionZ_max;
	}

	// Calculating the first intersection points
	Seg_ToVector_V3F(p_ray, p_outIntersectionPoint);
	Vec_Mul_3f_1f(p_outIntersectionPoint, l_rayDistanceFraction_min, p_outIntersectionPoint);
	Vec_Add_3f_3f(p_outIntersectionPoint, &p_ray->Begin, p_outIntersectionPoint);
	return true;
};

bool Intersection_AABB_Sphere(const BoxF_PTR p_AABB, const Spheref_PTR p_sphere)
{
	// We get the nearest point on the box to the sphere center.
	Vector3f l_nearestPoint;
	if (fabsf(p_sphere->Center.x - (p_AABB->Center.x + p_AABB->Extend.x)) >= fabsf(p_sphere->Center.x - (p_AABB->Center.x - p_AABB->Extend.x)))
	{
		l_nearestPoint.x = p_AABB->Center.x - p_AABB->Extend.x;
	}
	else
	{
		l_nearestPoint.x = p_AABB->Center.x + p_AABB->Extend.x;
	}

	if (fabsf(p_sphere->Center.y - (p_AABB->Center.y + p_AABB->Extend.y)) >= fabsf(p_sphere->Center.y - (p_AABB->Center.y - p_AABB->Extend.y)))
	{
		l_nearestPoint.y = p_AABB->Center.y - p_AABB->Extend.y;
	}
	else
	{
		l_nearestPoint.y = p_AABB->Center.y + p_AABB->Extend.y;
	}

	if (fabsf(p_sphere->Center.z - (p_AABB->Center.z + p_AABB->Extend.z)) >= fabsf(p_sphere->Center.z - (p_AABB->Center.z - p_AABB->Extend.z)))
	{
		l_nearestPoint.z = p_AABB->Center.z - p_AABB->Extend.z;
	}
	else
	{
		l_nearestPoint.z = p_AABB->Center.z + p_AABB->Extend.z;
	}
	return Vec_Distance_3f(&l_nearestPoint, &p_sphere->Center) <= p_sphere->Radius * p_sphere->Radius;
};

bool Contains_AABB_Sphere(const BoxF_PTR p_AABB, const Spheref_PTR p_sphere)
{
	// If sphere center is in the box (the sphere may be entirely contained in the box)
	return (Compare_float_float_value(p_sphere->Center.x, p_AABB->Center.x + p_AABB->Extend.x) <= 0 && Compare_float_float_value(p_sphere->Center.x, p_AABB->Center.x - p_AABB->Extend.x) >= 0
		|| Compare_float_float_value(p_sphere->Center.y, p_AABB->Center.y + p_AABB->Extend.y) <= 0 && Compare_float_float_value(p_sphere->Center.y, p_AABB->Center.y - p_AABB->Extend.y) >= 0
		|| Compare_float_float_value(p_sphere->Center.z, p_AABB->Center.z + p_AABB->Extend.z) <= 0 && Compare_float_float_value(p_sphere->Center.z, p_AABB->Center.z - p_AABB->Extend.z) >= 0);
};

bool Intersection_Contains_Plane_Segment(const Plane_PTR p_plane, const Segment_Vector3f_PTR p_segment)
{
	Vector3f tmp_vec3_0;
	Vec_Min_3f_3f(&p_segment->Begin, &p_plane->Point, &tmp_vec3_0);
	float l_beginDot = Vec_Dot_3f(&p_plane->Normal, &tmp_vec3_0);

	Vec_Min_3f_3f(&p_segment->End, &p_plane->Point, &tmp_vec3_0);
	float l_endDot = Vec_Dot_3f(&p_plane->Normal, &tmp_vec3_0);
	return ((l_beginDot >= FLOAT_TOLERANCE && l_endDot >= FLOAT_TOLERANCE)
		|| (l_beginDot >= FLOAT_TOLERANCE && l_endDot <= -FLOAT_TOLERANCE)
		|| (l_beginDot <= -FLOAT_TOLERANCE && l_endDot >= FLOAT_TOLERANCE));
};

bool Intersection_Contains_Frustum_Sphere(const Frustum_PTR p_frustum, const Spheref_PTR p_frustumProjectedSphere)
{
	Vector3f l_boxDelta_axis;
	Segment_Vector3f l_sphereProjected_x;
	l_boxDelta_axis = (Vector3f){ p_frustumProjectedSphere->Radius, 0.0f, 0.0f };
	Vec_Add_3f_3f(&p_frustumProjectedSphere->Center, &l_boxDelta_axis, &l_sphereProjected_x.End);
	l_boxDelta_axis = (Vector3f){ -p_frustumProjectedSphere->Radius, 0.0f, 0.0f };
	Vec_Add_3f_3f(&p_frustumProjectedSphere->Center, &l_boxDelta_axis, &l_sphereProjected_x.Begin);
	if (Intersection_Contains_Plane_Segment(&p_frustum->Right, &l_sphereProjected_x) && Intersection_Contains_Plane_Segment(&p_frustum->Left, &l_sphereProjected_x))
	{
		Segment_Vector3f l_sphereProjected_y;
		l_boxDelta_axis = (Vector3f) { 0.0f, p_frustumProjectedSphere->Radius, 0.0f };
		Vec_Add_3f_3f(&p_frustumProjectedSphere->Center, &l_boxDelta_axis, &l_sphereProjected_y.End);
		l_boxDelta_axis = (Vector3f) { 0.0f, -p_frustumProjectedSphere->Radius, 0.0f };
		Vec_Add_3f_3f(&p_frustumProjectedSphere->Center, &l_boxDelta_axis, &l_sphereProjected_y.Begin);
		if (Intersection_Contains_Plane_Segment(&p_frustum->Up, &l_sphereProjected_y) && Intersection_Contains_Plane_Segment(&p_frustum->Bottom, &l_sphereProjected_y))
		{
			Segment_Vector3f l_sphereProjected_z;
			l_boxDelta_axis = (Vector3f){ 0.0f, 0.0f, p_frustumProjectedSphere->Radius };
			Vec_Add_3f_3f(&p_frustumProjectedSphere->Center, &l_boxDelta_axis, &l_sphereProjected_z.End);
			l_boxDelta_axis = (Vector3f){ 0.0f, 0.0f, -p_frustumProjectedSphere->Radius };
			Vec_Add_3f_3f(&p_frustumProjectedSphere->Center, &l_boxDelta_axis, &l_sphereProjected_z.Begin);
			if (Intersection_Contains_Plane_Segment(&p_frustum->Near, &l_sphereProjected_z) && Intersection_Contains_Plane_Segment(&p_frustum->Far, &l_sphereProjected_z))
			{
				return true;
			}
		}
	}
	return false;
};

typedef char RECT_INTERSECTION_INTERNAL_TEST;
#define RECT_INTERSECTION_TEST_X_NOTHING 1
#define RECT_INTERSECTION_TEST_Y_NOTHING 2
#define RECT_INTERSECTION_TEST_X_INTERSECTION 4
#define RECT_INTERSECTION_TEST_Y_INTERSECTION 8
#define RECT_INTERSECTION_TEST_X_CONTAINS 16
#define RECT_INTERSECTION_TEST_Y_CONTAINS 32

RECT_INTERSECTION_TEST Intersection_Recti_Recti(const Recti_PTR p_source, const Recti_PTR p_target)
{
	RECT_INTERSECTION_INTERNAL_TEST l_x_intersection = RECT_INTERSECTION_TEST_X_NOTHING;

	if (p_target->Min.x < p_source->Max.x)
	{
		if (p_target->Max.x > p_source->Max.x)
		{
			l_x_intersection = RECT_INTERSECTION_TEST_X_INTERSECTION;
		}
		else if (p_target->Min.x < p_source->Min.x)
		{
			if (p_target->Max.x > p_source->Min.x)
			{
				l_x_intersection = RECT_INTERSECTION_TEST_X_INTERSECTION;
			}
			else
			{
				l_x_intersection = RECT_INTERSECTION_TEST_X_CONTAINS;
			}
		}
	}


	RECT_INTERSECTION_INTERNAL_TEST l_y_intersection = RECT_INTERSECTION_TEST_Y_NOTHING;
	if (p_target->Min.y < p_source->Max.y)
	{
		if (p_target->Max.y > p_source->Max.y)
		{
			l_y_intersection = RECT_INTERSECTION_TEST_Y_INTERSECTION;
		}
		else if(p_target->Min.y < p_source->Min.y)
		{
			if (p_target->Max.y > p_source->Min.y)
			{
				l_y_intersection = RECT_INTERSECTION_TEST_Y_INTERSECTION;
			}
			else
			{
				l_y_intersection = RECT_INTERSECTION_TEST_Y_CONTAINS;
			}
		}
	}

	RECT_INTERSECTION_INTERNAL_TEST l_intersectionSum = l_x_intersection + l_y_intersection;
	if (l_intersectionSum == (RECT_INTERSECTION_TEST_X_CONTAINS + RECT_INTERSECTION_TEST_Y_CONTAINS))
	{
		return RECT_INTERSECTION_TEST_CONTAINS;
	}
	else if (l_intersectionSum == (RECT_INTERSECTION_TEST_X_NOTHING + RECT_INTERSECTION_TEST_Y_NOTHING))
	{
		return RECT_INTERSECTION_TEST_NOTHING;
	}
	else
	{
		return RECT_INTERSECTION_TEST_INTERSECTION;
	}

};