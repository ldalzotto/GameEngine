#include "RayCast.h"

#include "Math/Transform/Transform.h"
#include "Math/Vector/Vector.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/Matrix.h"
#include "Math/Matrix/MatrixMath.h"
#include "Math/Box/Box.h"
#include "Math/Box/BoxMath.h"
#include <math.h>
#include "World/World.h"
#include "Collider/BoxCollider.h"

namespace _GameEngine::_Physics
{
	struct Segment
	{
		_Math::Vector3f Begin;
		_Math::Vector3f End;
	};

	_Math::Vector3f Segment_direction(Segment* p_segment)
	{
		_Math::Vector3f l_direction;
		_Math::Vector3f_min(&p_segment->End, &p_segment->Begin, &l_direction);
		_Math::Vector3f_normalize(&l_direction);
		return l_direction;
	};

	_Math::Vector3f Segment_toVector(Segment* p_segment)
	{
		_Math::Vector3f l_vector;
		_Math::Vector3f_min(&p_segment->End, &p_segment->Begin, &l_vector);
		return l_vector;
	};

	void Segment_mul(Segment* p_segment, _Math::Matrix4x4f* p_matrix, Segment* p_out)
	{
		_Math::Vector4f l_begin4f, l_end4f;
		{
			_Math::Vector4f_build(&p_segment->Begin, 1.0f, &l_begin4f);
			_Math::Vector4f_build(&p_segment->End, 1.0f, &l_end4f);
		}

		_Math::Vector4f l_projectedBegin, l_projectedEnd;
		_Math::Matrixf4x4_mul(p_matrix, &l_begin4f, &l_projectedBegin);
		_Math::Matrixf4x4_mul(p_matrix, &l_end4f, &l_projectedEnd);

		p_out->Begin = { l_projectedBegin.x, l_projectedBegin.y, l_projectedBegin.z };
		p_out->End = { l_projectedEnd.x, l_projectedEnd.y, l_projectedEnd.z };
	};

	void RayCastAll(World* p_world, _Math::Vector3f* p_begin, _Math::Vector3f* p_end, _Core::VectorT<_Math::Vector3f>* out_intersectionPoints)
	{
		Segment l_segment;
		{
			l_segment.Begin = *p_begin;
			l_segment.End = *p_end;
		}

		for (size_t i = 0; i < p_world->BoxColliders.size(); i++)
		{
			BoxCollider* l_boxCollider = *p_world->BoxColliders.at(i);
			_Math::Matrix4x4f l_worldToLocal = _Math::Transform_getWorldToLocalMatrix(l_boxCollider->Transform);

			Segment l_localProjectedSegment;
			Segment_mul(&l_segment, &l_worldToLocal, &l_localProjectedSegment);

			// Simple AABB check
			_Math::BoxPoints l_localBoxPoints;
			_Math::Box_extractPoints(l_boxCollider->Box, &l_localBoxPoints);

			_Math::Vector3f l_fragmentInsideBoxVector;

			float l_tmin, l_tmax;

			// X axis
			float l_t0x, l_t1x;
			{
				float l_xMax = l_localBoxPoints.R_D_B.x;
				float l_xMin = l_localBoxPoints.L_D_B.x;

				l_t0x = (l_xMin - l_localProjectedSegment.Begin.x) / (l_localProjectedSegment.End.x - l_localProjectedSegment.Begin.x);
				l_t1x = (l_xMax - l_localProjectedSegment.Begin.x) / (l_localProjectedSegment.End.x - l_localProjectedSegment.Begin.x);
			}

			l_tmin = l_t0x;
			l_tmax = l_t1x;

			if (l_tmin > l_tmax)
			{
				float l_tmp = l_tmax;
				l_tmax = l_tmin;
				l_tmin = l_tmp;
			}

			float l_t0y, l_t1y;
			{
				float l_yMax = l_localBoxPoints.R_U_B.y;
				float l_yMin = l_localBoxPoints.L_D_B.y;

				l_t0y = (l_yMin - l_localProjectedSegment.Begin.y) / (l_localProjectedSegment.End.y - l_localProjectedSegment.Begin.y);
				l_t1y = (l_yMax - l_localProjectedSegment.Begin.y) / (l_localProjectedSegment.End.y - l_localProjectedSegment.Begin.y);
			}

			if (l_t0y > l_t1y)
			{
				float l_tmp = l_t1y;
				l_t1y = l_t0y;
				l_t0y = l_tmp;
			}

			if ((l_tmin > l_t1y) || (l_t0y > l_tmax))
			{
				break;
			}

			if (l_t0y > l_tmin) {
				l_tmin = l_t0y;
			}
			if (l_t1y < l_tmax) {
				l_tmax = l_t1y;
			}

			float l_t0z, l_t1z;
			{
				float l_zMax = l_localBoxPoints.R_D_F.z;
				float l_zMin = l_localBoxPoints.L_D_B.z;

				l_t0z = (l_zMin - l_localProjectedSegment.Begin.z) / (l_localProjectedSegment.End.z - l_localProjectedSegment.Begin.z);
				l_t1z = (l_zMax - l_localProjectedSegment.Begin.z) / (l_localProjectedSegment.End.z - l_localProjectedSegment.Begin.z);
			}

			if (l_t0z > l_t1z)
			{
				float l_tmp = l_t1z;
				l_t1z = l_t0z;
				l_t0z = l_tmp;
			}

			if ((l_tmin > l_t1z) || (l_t0z > l_tmax))
			{
				break;
			}

			if (l_t0z > l_tmin)
			{
				l_tmin = l_t0z;
			}
			if (l_t1z < l_tmax)
			{
				l_tmax = l_t1z;
			}


			// Calculating intersection points
			{
				_Math::Vector3f l_intersection1Local = Segment_toVector(&l_localProjectedSegment);
				_Math::Vector3f_mul(&l_intersection1Local, l_tmin, &l_intersection1Local);
				_Math::Vector3f_add(&l_intersection1Local, &l_localProjectedSegment.Begin, &l_intersection1Local);

				_Math::Vector3f l_intersection1World, l_intersection2World;
				_Math::Matrix4x4f* l_localToWorld = _Math::Transform_getLocalToWorldMatrix_ref(l_boxCollider->Transform);
				_Math::Matrixf4x4_mul(l_localToWorld, &l_intersection1Local, &l_intersection1World);

				out_intersectionPoints->push_back(&l_intersection1World);
			}
		}
	};
}