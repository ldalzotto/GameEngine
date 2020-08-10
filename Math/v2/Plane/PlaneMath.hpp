#pragma once

#include "Plane.hpp"

#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	struct PlaneM
	{
		static Plane build(const float p_a, const float p_b, const float p_c, const float p_d);
		static Plane build(const Vector3<float>* p_0, const  Vector3<float>* p_1, const  Vector3<float>* p_2);
		static float minDistance(const Plane* p_plane, const Vector3<float>* p_point);
	};
}