#pragma once

namespace _MathV2
{
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

	extern const Vector3<float> RIGHT;
	extern const Vector3<float> UP;
	extern const Vector3<float> FORWARD;

}