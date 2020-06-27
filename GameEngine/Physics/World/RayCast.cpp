#include "RayCast.h"

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

	void RayCastAll(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Core::VectorT<RaycastHit>* out_intersectionPoints)
	{
		_Math::Segment l_segment;
		{
			l_segment.Begin = *p_begin;
			l_segment.End = *p_end;
		}

		for (size_t i = 0; i < p_world->BoxColliders.size(); i++)
		{
			BoxCollider* l_boxCollider = *p_world->BoxColliders.at(i);
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
				out_intersectionPoints->push_back(&hit);
			}
		}
	};

	bool RayCast(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, RaycastHit* out_hit)
	{
		bool l_return = false;
		_Core::VectorT<RaycastHit> l_hits;
		l_hits.alloc();
		{
			RayCastAll(p_world, p_begin, p_end, &l_hits);

			if (l_hits.size() > 0)
			{
				l_return = true;

				// We get the nearest hit from begin
				size_t l_currentIndex = 0;
				float l_comparedDistacne = _Math::Vector3f_distance(p_begin, &(l_hits.at(l_currentIndex))->HitPoint);

				for (size_t i = 1; i < l_hits.size(); i++)
				{
					float l_dst = _Math::Vector3f_distance(p_begin, &(l_hits.at(i))->HitPoint);
					if (l_dst <= l_comparedDistacne)
					{
						l_currentIndex = i;
						l_comparedDistacne = l_dst;
					}
				}

				*out_hit = *l_hits.at(l_currentIndex);
			}

		}
		l_hits.free();

		return l_return;
	};
}