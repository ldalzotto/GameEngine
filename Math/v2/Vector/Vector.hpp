#pragma once

#include "v2/Vector/Vector.h"

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

		static inline Vector<3, T> build(T p_x, T p_y, T p_z)
		{
			return Vector<3, T>{p_x, p_y, p_z};
		};

		inline float length()
		{
			return RVector_3_length((T*)(this));
		};

		inline Vector<3, T> normalize()
		{
			Vector3<T> l_out;
			RVector_3_normalize((T*)(this), (T*)(&l_out));
			return l_out;
		};
		
		inline Vector<3, T> add(Vector<3, T>& p_other)
		{
			Vector3<T> l_out;
			RVector_3_add((T*)this, (T*)(&p_other), (T*)&l_out);
			return l_out;
		};

		inline Vector<3, T> mul(T p_scal)
		{
			Vector<3, T> l_out;
			RVector_3_mul((T*)(this), p_scal, (T*)(&l_out));
			return l_out;
		};

		inline float dot(Vector<3, T>& p_right)
		{
			return RVector_3_dot((T*)(this), (T*)(&p_right));
		};

		inline Vector<3, T> cross(Vector<3, T>& p_right)
		{
			Vector<3, T> l_out;
			RVector_3_cross((T*)(this), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};

		inline float angle(Vector<3, T>& p_end)
		{
			return RVector_3_angle((T*)this, (T*)(&p_end));
		}

		inline Vector<3, T> rotate(Quaternion<T>& p_rotation)
		{
			Vector<3, T> l_out;
			RVector_3_rotate((T*)(this), (T*)(&p_rotation), (T*)(&l_out));
			return l_out;
		};
	};

	template <typename T>
	using Vector3 = Vector<3, T>;

	template <typename T>
	struct Vector<4, T>
	{
		T x, y, z, w;

		inline float length()
		{
			return RVector_4_length((T*)(this));
		};

		inline Vector<4, T> mul(T p_right)
		{
			Vector<4, T> l_out;
			RVector_4_mul((T*)this, p_right, (T*)(&l_out));
			return l_out;
		};
	};

	template <typename T>
	using Vector4 = Vector<4, T>;

	Vector3<float> RIGHT = { RIGHT_arr[0], RIGHT_arr[1], RIGHT_arr[2] };
	Vector3<float> UP = { UP_arr[0], UP_arr[1], UP_arr[2] };
	Vector3<float> FORWARD = { FORWARD_arr[0], FORWARD_arr[1], FORWARD_arr[2] };

}