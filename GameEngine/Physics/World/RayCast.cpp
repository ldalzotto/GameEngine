#include "RayCast.h"

#include "Functional/Sort/ElementSorter.hpp"
#include "Algorithm/Sort/SortAlgorithmT.hpp"
#include "DataStructures/Specifications/ArrayT.hpp"

#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Intersection/Intersection.h"
#include "v2/Transform/TransformM.hpp"
#include "v2/Box/BoxMath.h"
#include "v2/Segment/SegmentMath.hpp"
#include <math.h>
#include "World/World.h"
#include "Collider/BoxCollider.h"

using namespace _MathV2;

namespace _GameEngine::_Physics
{
	struct RaycastHitDistanceComparatorObject
	{
		_MathV2::Vector3<float> RayBegin;
		bool DistanceCalculated;
		float CachedDistance;
	};

	short RaycastHit_distanceMinComparator(RaycastHit* p_left, RaycastHit* p_right, RaycastHitDistanceComparatorObject* p_comparatorObject)
	{
		float l_leftDistance = 0.0f;
		if (!p_comparatorObject->DistanceCalculated)
		{
			l_leftDistance =  VectorM::distance(p_comparatorObject->RayBegin, p_left->HitPoint);
		}
		else
		{
			l_leftDistance = p_comparatorObject->CachedDistance;
		}

		float l_rightDistance = VectorM::distance(p_comparatorObject->RayBegin, p_right->HitPoint);
		short l_comparisonResult = _Core::SortCompare_float_float(&l_leftDistance, &l_rightDistance);

		if (l_comparisonResult >= 0) { p_comparatorObject->CachedDistance = l_leftDistance; p_comparatorObject->DistanceCalculated = true; }
		return l_comparisonResult;
	};
}

namespace _GameEngine::_Physics
{

	void RayCastAll(World* p_world, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints)
	{
		RayCastAll_against((_Core::ArrayT<BoxCollider*>*)&p_world->BoxColliders, p_begin, p_end, out_intersectionPoints);
	};

	bool RayCast(World* p_world, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, RaycastHit* out_hit)
	{
		return RayCast_against((_Core::ArrayT<BoxCollider*>*) & p_world->BoxColliders, p_begin, p_end, out_hit);
	};

	void RayCastAll_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints)
	{
		Segment l_segment;
		{
			l_segment.Begin = p_begin;
			l_segment.End = p_end;
		}

		auto l_boxCollidersIt = _Core::ArrayT_buildIterator(p_comparedColliders);
		while (_Core::VectorIteratorT_moveNext(&l_boxCollidersIt))
		{
			BoxCollider* l_boxCollider = (*l_boxCollidersIt.Current);
			_MathV2::Matrix4x4<float> l_worldToLocal = TransformM::getWorldToLocalMatrix(*l_boxCollider->Transform);

			// We project the ray to the box local space, to perform an AABB test.
			_MathV2::Vector3<float> l_intersectionPointLocal;
			if (Intersection_AABB_Ray(*l_boxCollider->Box, SegmentM::mul(l_segment, l_worldToLocal), &l_intersectionPointLocal))
			{
				RaycastHit hit{};

				// The intersection point is then projected back to world space.
				hit.HitPoint = VectorM::cast(MatrixM::mul(TransformM::getLocalToWorldMatrix(*l_boxCollider->Transform), VectorM::cast(l_intersectionPointLocal, 1.0f)));
				hit.Collider = l_boxCollider;
				_Core::VectorT_pushBack(out_intersectionPoints, &hit);
			}
		}
	};
	
	bool RayCast_against(_Core::ArrayT<_Physics::BoxCollider*>* p_comparedColliders, _MathV2::Vector3<float>& p_begin, _MathV2::Vector3<float>& p_end, RaycastHit* out_hit)
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
				l_raycastMinComparatorObject.RayBegin = p_begin;
				*out_hit = *_Core::SortT_min(_Core::VectorT_buildIterator(&l_hits), 0,
					_Core::ElementSorterT<RaycastHit, RaycastHit, RaycastHitDistanceComparatorObject> { RaycastHit_distanceMinComparator, & l_raycastMinComparatorObject });
			}

		}
		_Core::VectorT_free(&l_hits);

		return l_return;
	};
}