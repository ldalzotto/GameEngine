#include "Intersection.h"

#include "Math/Vector/VectorMath.h"
#include "Math/Box/Box.h"
#include "Math/Box/BoxMath.h"
#include "Math/Segment/Segment.h"

namespace _GameEngine::_Math
{
	bool Intersection_AABB_Ray(Box* p_AABB, Segment* p_ray, Vector3f* p_outIntersectionPoint)
	{
		_MathV2::Vector3<float> l_boxMin, l_boxMax;
		_Math::Box_extractMinMax(p_AABB, l_boxMin, l_boxMax);

		_Math::Vector3f l_fragmentInsideBoxVector;

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
		*p_outIntersectionPoint = Segment_toVector(p_ray);
		_Math::Vector3f_mul(p_outIntersectionPoint, l_rayDistanceFraction_min, p_outIntersectionPoint);
		_Math::Vector3f_add(p_outIntersectionPoint, &p_ray->Begin, p_outIntersectionPoint);
		return true;
	};

}