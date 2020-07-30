#pragma once

#include "Vector.hpp"

namespace _MathV2
{
	struct VectorM
	{
		template <typename T>
		inline static Vector<3, T> build(T p_x, T p_y, T p_z)
		{
			return Vector<3, T>{p_x, p_y, p_z};
		};
		template <typename T>
		inline static float length(Vector<3, T>& p_vec)
		{
			return RVector_3_length((T*)(&p_vec));
		};
		template <typename T>
		inline static Vector<3, T> normalize(Vector<3, T>& p_vec)
		{
			Vector3<T> l_out;
			RVector_3_normalize((T*)(&p_vec), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Vector<3, T> add(Vector<3, T>& p_left, Vector<3, T>& p_right)
		{
			Vector3<T> l_out;
			RVector_3_add((T*)(&p_left), (T*)(&p_right), (T*)&l_out);
			return l_out;
		};
		template <typename T>
		inline static Vector<3, T> min(Vector<3, T>& p_left, Vector<3, T>& p_right)
		{
			Vector3<T> l_out;
			RVector_3_min((T*)(&p_left), (T*)(&p_right), (T*)&l_out);
			return l_out;
		};
		template <typename T>
		inline static Vector<3, T> mul(Vector<3, T>& p_left, T p_right)
		{
			Vector<3, T> l_out;
			RVector_3_mul((T*)(&p_left), p_right, (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static float dot(Vector<3, T>& p_left, Vector<3, T>& p_right)
		{
			return RVector_3_dot((T*)(&p_left), (T*)(&p_right));
		};
		template <typename T>
		inline static Vector<3, T> cross(Vector<3, T>& p_left, Vector<3, T>& p_right)
		{
			Vector<3, T> l_out;
			RVector_3_cross((T*)(&p_left), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static float angle(Vector<3, T>& p_begin, Vector<3, T>& p_end)
		{
			return RVector_3_angle((T*)(&p_begin), (T*)(&p_end));
		};
		template <typename T>
		inline static float angle_normalized(Vector<3, T>& p_begin, Vector<3, T>& p_end)
		{
			return RVector_3_angle_normalized((T*)(&p_begin), (T*)(&p_end));
		};
		template <typename T>
		inline static short int angleSign(Vector<3, T>& p_begin, Vector<3, T>& p_end, Vector<3, T>& p_referenceAxis)
		{
			return RVector_3_angleSign((T*)(&p_begin), (T*)(&p_end), (T*)(&p_referenceAxis));
		};
		template <typename T>
		inline static Vector<3, T> rotate(Vector<3, T>& p_vec, Quaternion<T>& p_rotation)
		{
			Vector<3, T> l_out;
			RVector_3_rotate((T*)(&p_vec), (T*)(&p_rotation), (T*)(&l_out));
			return l_out;
		};

		template <typename T>
		inline static bool equals(Vector<3, T>& p_left, Vector<3, T>& p_right)
		{
			return RVector_3_equals((T*)(&p_left), (T*)(&p_right));
		};

		template <typename T>
		inline static float length(Vector<4, T>& p_vec)
		{
			return RVector_4_length((T*)(&p_vec));
		};

		template <typename T>
		inline static Vector<4, T> mul(Vector<4, T>& p_left, T p_right)
		{
			Vector<4, T> l_out;
			RVector_4_mul((T*)(&p_left), p_right, (T*)(&l_out));
			return l_out;
		};
	};
}