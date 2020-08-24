#include "RayCast.h"

#include "Functional/Compare.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/Intersection.h"
#include "v2/_interface/SegmentC.h"
#include "v2/_interface/TransformC.h"
#include "v2/_interface/VectorC.h"
#include "Collider/BoxCollider.h"
#include "Algorithm/Sort.h"

#include <math.h>
#include "World/World.h"

typedef struct RaycastHitDistanceComparatorObject_TYP
{
	Vector3f RayBegin;
	bool DistanceCalculated;
	float CachedDistance;
}RaycastHitDistanceComparatorObject, * RaycastHitDistanceComparatorObject_PTR;

SORT_SELECTION_FUNCTION(RaycastHit, Array_RaycastHit_PTR, RaycastHit, RaycastHitDistanceComparatorObject*)

short RaycastHit_distanceMinComparator(RaycastHit* p_left, RaycastHit* p_right, RaycastHitDistanceComparatorObject* p_comparatorObject)
{
	float l_leftDistance = 0.0f;
	if (!p_comparatorObject->DistanceCalculated)
	{
		l_leftDistance = Vec_Distance_3f(&p_comparatorObject->RayBegin, &p_left->HitPoint);
	}
	else
	{
		l_leftDistance = p_comparatorObject->CachedDistance;
	}

	float l_rightDistance = Vec_Distance_3f(&p_comparatorObject->RayBegin, &p_right->HitPoint);
	short l_comparisonResult = Compare_float_float(&l_leftDistance, &l_rightDistance);

	if (l_comparisonResult >= 0) { p_comparatorObject->CachedDistance = l_leftDistance; p_comparatorObject->DistanceCalculated = true; }
	return l_comparisonResult;
};



void RayCastAll_Realloc(World* p_world, Vector3f_PTR p_begin, Vector3f_PTR p_end, Array_RaycastHit_PTR out_intersectionPoints)
{
	RayCastAll_Against_Realloc(&p_world->BoxColliders, p_begin, p_end, out_intersectionPoints);
};

bool RayCast(World* p_world, Vector3f_PTR p_begin, Vector3f_PTR p_end, RaycastHit* out_hit)
{
	return RayCast_Against(&p_world->BoxColliders, p_begin, p_end, out_hit);
};

void RayCastAll_Against_Realloc(Array_BoxColliderPTR_PTR p_comparedColliders, Vector3f_PTR p_begin, Vector3f_PTR p_end, Array_RaycastHit_PTR out_intersectionPoints)
{
	Segment_Vector4f l_segment;
	{
		l_segment.Begin.Vec3 = *p_begin; l_segment.Begin.Vec3_w = 1.0f;
		l_segment.End.Vec3 = *p_end; l_segment.End.Vec3_w = 1.0f;
	}

	for (size_t i = 0; i < p_comparedColliders->Size; i++)
	{
		Segment_Vector4f tmp_segment_4f; Segment_Vector3f tmp_segment_3f; Matrix4f tmp_mat4_0;

		BoxCollider* l_boxCollider = p_comparedColliders->Memory[i];

		// We project the ray to the box local space, to perform an AABB test.
		Vector3f l_intersectionPointLocal;
		Transform_GetWorldToLocalMatrix(l_boxCollider->Transform, &tmp_mat4_0);
		Seg_Mul_V4F_M4F(&l_segment, &tmp_mat4_0, &tmp_segment_4f);
		tmp_segment_3f.Begin = tmp_segment_4f.Begin.Vec3; tmp_segment_3f.End = tmp_segment_4f.End.Vec3;
		if (Intersection_AABB_Ray((BoxF_PTR)l_boxCollider->Box, &tmp_segment_3f, &l_intersectionPointLocal))
		{
			RaycastHit hit = {0};

			// The intersection point is then projected back to world space.
			Vector4f tmp_vec4, tmp_vec4_1;
			Transform_GetLocalToWorldMatrix(l_boxCollider->Transform, &tmp_mat4_0);
			tmp_vec4.Vec3 = l_intersectionPointLocal; tmp_vec4.Vec3_w = 1.0f;
			Mat_Mul_M4F_V4F(&tmp_mat4_0, &tmp_vec4, &tmp_vec4_1);
			hit.HitPoint = tmp_vec4_1.Vec3;
			hit.Collider = l_boxCollider;
			Arr_PushBackRealloc_RaycastHit(out_intersectionPoints, &hit);
		}
	}
};

bool RayCast_Against(Array_BoxColliderPTR_PTR p_comparedColliders, Vector3f_PTR p_begin, Vector3f_PTR p_end, RaycastHit* out_hit)
{
	bool l_return = false;
	Array_RaycastHit  l_hits;
	Arr_Alloc_RaycastHit(&l_hits, 0);
	{
		RayCastAll_Against_Realloc(p_comparedColliders, p_begin, p_end, &l_hits);

		if (l_hits.Size > 0)
		{
			// We use a return value instead of directly returning to free the l_hits vector.
			l_return = true;
			RaycastHitDistanceComparatorObject l_raycastMinComparatorObject = {0};
			l_raycastMinComparatorObject.RayBegin = *p_begin;
			Sort_SelectionSort_RaycastHit(&l_hits, RaycastHit_distanceMinComparator, &l_raycastMinComparatorObject);
			*out_hit = l_hits.Memory[0];
		}

	}
	Arr_Free((Array_PTR)&l_hits);
	return l_return;
};