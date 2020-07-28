#pragma once

#include <math.h>
#include "Math.hpp"

namespace _Math
{
	template <typename T>
	struct Vector2
	{
		T x, y;
	};

	template <typename T>
	struct Vector3
	{
		T x, y, z;
	};

	const Vector3<float> FORWARD = { 0.0f, 0.0f, 1.0f };

	template <typename T>
	Vector3<T> Vector3_build(T& p_x, T& p_y, T& p_z)
	{
		return Vector3<T>{p_x, p_y, p_z};
	};

	template <typename T>
	Vector3<T> Vector3_add(Vector3<T>& p_left, Vector3<T>& p_right)
	{
		return Vector3<T>
		{
			p_left.x + p_right.x,
				p_left.y + p_right.y,
				p_left.z + p_right.z
		};
	}

	template <typename T>
	Vector3<T> Vector3_add(Vector3<T>& p_left, Vector3<T>& p_right_1, Vector3<T>& p_right_2)
	{
		return Vector3_add(p_left, Vector3_add(p_right_1, p_right_2));
	}

	template <typename T>
	Vector3<T> Vector3_mul(Vector3<T>& p_left, T p_right)
	{
		return Vector3<T>
		{
			p_left.x* p_right,
				p_left.y* p_right,
				p_left.z* p_right
		};
	};

	template <typename T>
	Vector3<T> Vector3_normalize(Vector3<T>& p_vec)
	{
		float l_length = Vector3_length(p_vec);
		return Vector3<T>
		{
			p_vec.x / l_length,
			p_vec.y / l_length,
			p_vec.z / l_length
		};
	};

	template <typename T>
	Vector3<T> Vector3_normalize(Vector3<T>&& p_vec)
	{
		return Vector3_normalize(p_vec);
	};

	template <typename T>
	float Vector3_dot(Vector3<T>& p_left, Vector3<T>& p_right)
	{
		return
			(p_left.x * p_right.x) +
			(p_left.y * p_right.y) +
			(p_left.z * p_right.z);
	};

	template <typename T>
	Vector3<T> Vector3_cross(Vector3<T>& p_left, Vector3<T>& p_right)
	{
		return Vector3<T>
		{
			(p_left.y * p_right.z) - (p_left.z * p_right.y),
				(p_left.z* p_right.x) - (p_left.x * p_right.z),
				(p_left.x* p_right.y) - (p_left.y * p_right.x)
		};
	};

	template <typename T>
	float Vector3_length(Vector3<T>& p_vec)
	{
		return
			sqrt<T, float>((p_vec.x * p_vec.x) +
				(p_vec.y * p_vec.y) +
				(p_vec.z * p_vec.z));
	};

	template <typename T>
	float Vector3_angle(Vector3<T>& p_begin, Vector3<T>& p_end)
	{
		acosf(
			Vector3_dot(p_begin, p_end) / (Vector3_length(p_begin) * Vector3_length(p_end))
		);
	}


	template <typename T>
	struct Vector4
	{
		T x, y, z, w;
	};

	template <typename T>
	float Vector4_length(Vector4<T>& p_vec)
	{
		return
			sqrt<T, float>((p_vec.x * p_vec.x) +
				(p_vec.y * p_vec.y) +
				(p_vec.z * p_vec.z) +
				(p_vec.w * p_vec.w));
	};

	template <typename T>
	Vector4<T> Vector4_mul(Vector4<T>& p_left, T p_right)
	{
		return Vector4<T>
		{
			p_left.x* p_right,
				p_left.y* p_right,
				p_left.z* p_right,
				p_left.w* p_right
		};
	};
}