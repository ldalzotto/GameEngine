#include "RayCast.h"

#include "Functional/Sort/ElementSorter.hpp"
#include "Algorithm/Sort/SortAlgorithmT.hpp"
#include "DataStructures/Specifications/ArrayT.hpp"

extern "C"
{
#include "Functional.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/Intersection.h"
#include "v2/_interface/SegmentC.h"
#include "v2/_interface/TransformC.h"
#include "v2/_interface/VectorC.h"
}
#include <math.h>
#include "World/World.h"
#include "Collider/BoxCollider.h"


namespace _GameEngine::_Physics
{
	struct RaycastHitDistanceComparatorObject
	{
		VECTOR3F RayBegin;
		bool DistanceCalculated;
		float CachedDistance;
	};

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
}

namespace _GameEngine::_Physics
{

	void RayCastAll(World* p_world, VECTOR3F_PTR p_begin, VECTOR3F_PTR p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints)
	{
		RayCastAll_against((_Core::ArrayT<BoxCollider*>*) & p_world->BoxColliders, p_begin, p_end, out_intersectionPoints);
	};

	bool RayCast(World* p_world, VECTOR3F_PTR p_begin, VECTOR3F_PTR p_end, RaycastHit* out_hit)
	{
		return RayCast_against((_Core::ArrayT<BoxCollider*>*) & p_world->BoxColliders, p_begin, p_end, out_hit);
	};

	void RayCastAll_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, VECTOR3F_PTR p_begin, VECTOR3F_PTR p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints)
	{
		SEGMENT_VECTOR4F l_segment;
		{
			l_segment.Begin.Vec3 = *p_begin; l_segment.Begin.Vec3_w = 1.0f;
			l_segment.End.Vec3 = *p_end; l_segment.End.Vec3_w = 1.0f;
		}

		auto l_boxCollidersIt = _Core::ArrayT_buildIterator(p_comparedColliders);
		while (_Core::VectorIteratorT_moveNext(&l_boxCollidersIt))
		{
			SEGMENT_VECTOR4F tmp_segment_4f; SEGMENT_VECTOR3F tmp_segment_3f; MATRIX4F tmp_mat4_0;

			BoxCollider* l_boxCollider = (*l_boxCollidersIt.Current);

			// We project the ray to the box local space, to perform an AABB test.
			VECTOR3F l_intersectionPointLocal;
			Transform_GetWorldToLocalMatrix(l_boxCollider->Transform, &tmp_mat4_0);
			Seg_Mul_V4F_M4F(&l_segment, &tmp_mat4_0, &tmp_segment_4f);
			tmp_segment_3f.Begin = tmp_segment_4f.Begin.Vec3; tmp_segment_3f.End = tmp_segment_4f.End.Vec3;
			if (Intersection_AABB_Ray((BOXF_PTR)l_boxCollider->Box, &tmp_segment_3f, & l_intersectionPointLocal))
			{
				RaycastHit hit{};

				// The intersection point is then projected back to world space.
				VECTOR4F tmp_vec4, tmp_vec4_1;
				Transform_GetLocalToWorldMatrix(l_boxCollider->Transform, &tmp_mat4_0);
				tmp_vec4.Vec3 = l_intersectionPointLocal; tmp_vec4.Vec3_w = 1.0f;
				Mat_Mul_M4F_V4F(&tmp_mat4_0, &tmp_vec4, &tmp_vec4_1);
				hit.HitPoint = tmp_vec4_1.Vec3;
				hit.Collider = l_boxCollider;
				_Core::VectorT_pushBack(out_intersectionPoints, &hit);
			}
		}
	};

	bool RayCast_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, VECTOR3F_PTR p_begin, VECTOR3F_PTR p_end, RaycastHit* out_hit)
	{
		bool l_return = false;
		_Core::VectorT<RaycastHit> l_hits;
		_Core::VectorT_alloc(&l_hits, 0);
		{
			RayCastAll_against(p_comparedColliders, p_begin, p_end, &l_hits);

			if (l_hits.Size > 0)
			{
				// We use a return value instead of directly returning to free the l_hits vector.
				l_return = true;
				RaycastHitDistanceComparatorObject l_raycastMinComparatorObject{};
				l_raycastMinComparatorObject.RayBegin = *p_begin;
				*out_hit = *_Core::SortT_min(_Core::VectorT_buildIterator(&l_hits), 0,
					_Core::ElementSorterT<RaycastHit, RaycastHit, RaycastHitDistanceComparatorObject> { RaycastHit_distanceMinComparator, & l_raycastMinComparatorObject });
			}

		}
		_Core::VectorT_free(&l_hits);

		return l_return;
	};
}