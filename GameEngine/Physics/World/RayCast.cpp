#include "RayCast.h"

#include "Functional/Sort/ElementSorter.hpp"
#include "Algorithm/Sort/SortAlgorithmT.hpp"

#include "Math/Intersection/Intersection.h"
#include "Math/Transform/Transform.h"
#include "Math/Matrix/Matrix.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Box/Box.h"
#include "Math/Box/BoxMath.h"
#include "Math/Segment/Segment.h"
#include <math.h>
#include "World/World.h"
#include "Collider/BoxCollider.h"

namespace _GameEngine::_Physics
{
	struct RaycastHitDistanceComparatorObject
	{
		_Math::Vector3f* RayBegin;
		bool DistanceCalculated;
		float CachedDistance;
	};

	short RaycastHit_distanceMinComparator(RaycastHit* p_left, RaycastHit* p_right, RaycastHitDistanceComparatorObject* p_comparatorObject)
	{
		float l_leftDistance = 0.0f;
		if (!p_comparatorObject->DistanceCalculated)
		{
			l_leftDistance = _Math::Vector3f_distance(p_comparatorObject->RayBegin, &(p_left)->HitPoint);
		}
		else
		{
			l_leftDistance = p_comparatorObject->CachedDistance;
		}

		float l_rightDistance = _Math::Vector3f_distance(p_comparatorObject->RayBegin, &(p_right)->HitPoint);
		short l_comparisonResult = _Core::SortCompare_float_float(&l_leftDistance, &l_rightDistance);

		if (l_comparisonResult >= 0) { p_comparatorObject->CachedDistance = l_leftDistance; p_comparatorObject->DistanceCalculated = true; }
		return l_comparisonResult;
	};
}

namespace _GameEngine::_Physics
{

	void RayCastAll(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints)
	{
		_Math::Segment l_segment;
		{
			l_segment.Begin = *p_begin;
			l_segment.End = *p_end;
		}

		auto l_boxCollidersIt = _Core::VectorT_buildIterator(&p_world->BoxColliders);
		while (_Core::VectorIteratorT_moveNext(&l_boxCollidersIt))
		{
			BoxCollider* l_boxCollider = (*l_boxCollidersIt.Current);
			_Math::Matrix4x4f l_worldToLocal = _Math::Transform_getWorldToLocalMatrix(l_boxCollider->Transform);

			// We project the ray to the box local space, to perform an AABB test.
			_Math::Segment l_localProjectedSegment;
			Segment_mul(&l_segment, &l_worldToLocal, &l_localProjectedSegment);

			_Math::Vector3f l_intersectionPointLocal;
			if (_Math::Intersection_AABB_Ray(l_boxCollider->Box, &l_localProjectedSegment, &l_intersectionPointLocal))
			{
				RaycastHit hit{};

				// The intersection point is then projected back to world space.
				_Math::Matrix4x4f* l_localToWorld = _Math::Transform_getLocalToWorldMatrix_ref(l_boxCollider->Transform);
				_Math::Matrixf4x4_mul(l_localToWorld, &l_intersectionPointLocal, &hit.HitPoint);
				hit.Collider = l_boxCollider;
				_Core::VectorT_pushBack(out_intersectionPoints, &hit);
			}
		}
	};

	bool RayCast(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, RaycastHit* out_hit)
	{
		bool l_return = false;
		_Core::VectorT<RaycastHit> l_hits;
		_Core::VectorT_alloc(&l_hits, 0);
		{
			RayCastAll(p_world, p_begin, p_end, &l_hits);

			if (l_hits.Size > 0)
			{
				// We use a return value instead of directly returning to free the l_hits vector.
				l_return = true;
				RaycastHitDistanceComparatorObject l_raycastMinComparatorObject{};
				l_raycastMinComparatorObject.RayBegin = p_begin;
				*out_hit = *_Core::SortT_min(_Core::VectorT_buildIterator(&l_hits), 0,
							_Core::ElementSorterT<RaycastHit, RaycastHit, RaycastHitDistanceComparatorObject> { RaycastHit_distanceMinComparator, & l_raycastMinComparatorObject });
			}

		}
		_Core::VectorT_free(&l_hits);

		return l_return;
	};
}