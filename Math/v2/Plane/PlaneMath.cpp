#include "PlaneMath.hpp"

#include <math.h>
#include "v2/Vector/VectorMath.hpp"

namespace _MathV2
{
	Plane PlaneM::build(const float p_a, const float p_b, const float p_c, const float p_d)
	{
		Plane l_plane;
		l_plane.Normal = { p_a, p_b, p_c };
		VectorM::normalize(&l_plane.Normal, &l_plane.Normal);
		l_plane.Point = { 0.0f, 0.0f, p_d / p_c };
		return l_plane;
	};

	Plane PlaneM::build(const Vector3<float>* p_0, const Vector3<float>* p_1, const  Vector3<float>* p_2)
	{
		Plane l_plane;
		Vector3<float> l_01; VectorM::min(p_1, p_0, &l_01);
		Vector3<float> l_02; VectorM::min(p_2, p_0, &l_02);
		Vector3<float> l_cross;
		VectorM::cross(&l_01, &l_02, &l_plane.Normal);
		VectorM::normalize(&l_plane.Normal, &l_plane.Normal);
		l_plane.Point = *p_0;
		return l_plane;
	};

	float PlaneM::minDistance(const Plane* p_plane, const Vector3<float>* p_point)
	{
		Vector3<float> l_d;
		return fabsf(VectorM::dot(VectorM::min(p_point, &p_plane->Point, &l_d), &p_plane->Normal));
	};
}