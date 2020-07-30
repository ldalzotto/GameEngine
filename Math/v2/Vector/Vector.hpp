#pragma once

#include "v2/Vector/RVector.h"

namespace _MathV2
{
	template<class>
	struct Quaternion;

	template <int N, typename T>
	struct Vector {

	};

	template <typename T>
	struct Vector<2, T>
	{
		T x, y;
	};

	template <typename T>
	using Vector2 = Vector<2, T>;

	template <typename T>
	struct Vector<3, T>
	{
		T x, y, z;
	};

	template <typename T>
	using Vector3 = Vector<3, T>;

	template <typename T>
	struct Vector<4, T>
	{
		T x, y, z, w;
	};

	template <typename T>
	using Vector4 = Vector<4, T>;

	extern Vector3<float> RIGHT;
	extern Vector3<float> UP;
	extern Vector3<float> FORWARD;

}