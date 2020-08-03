#pragma once

#include "Vector.hpp"
#include "v2/Quaternion/Quaternion.hpp"
#include "RVector.h"

namespace _MathV2
{
	struct VectorM
	{
		template <typename T>
		inline static Vector<3, T>* cast(Vector<4, T>* p_vec)
		{
			return (Vector<3, T>*)p_vec;
		};

		template <typename T>
		inline static Vector<4, T> cast(const Vector<3, T>* p_vec, const  T p_scal)
		{
			return Vector<4, T>{p_vec->x, p_vec->y, p_vec->z, p_scal};
		};

		template <typename T>
		inline static Vector<2, T>* cast(Vector<3, T>* p_vec)
		{
			return (Vector<2, T>*)(p_vec);
		};

		template <typename T>
		inline static Vector<3, T> build(const T p_x, const T p_y, const T p_z)
		{
			return Vector<3, T>{p_x, p_y, p_z};
		};
		template <typename T>
		inline static float length(const Vector<3, T>* p_vec)
		{
			return RVector_3_length((T*)(p_vec));
		};
		template <typename T>
		inline static float distance(const Vector<3, T>* p_start, const Vector<3, T>* p_end)
		{
			return RVector_3_distance((T*)(p_start), (T*)(p_end));
		};
		template <typename T>
		inline static Vector<3, T>* normalize(const Vector<3, T>* p_vec, Vector<3, T>* p_out)
		{
			RVector_3_normalize((T*)(p_vec), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Vector<3, T>* add(const Vector<3, T>* p_left, const Vector<3, T>* p_right, Vector<3, T>* p_out)
		{
			RVector_3_add((T*)(p_left), (T*)(p_right), (T*)p_out);
			return p_out;
		};
		template <typename T>
		inline static Vector<2, T>* min(const Vector<2, T>* p_left, const Vector<2, T>* p_right, Vector<2, T>* p_out)
		{
			RVector_2_min((T*)(p_left), (T*)(p_right), (T*)p_out);
			return p_out;
		};
		template <typename T>
		inline static Vector<3, T>* min(const Vector<3, T>* p_left, const Vector<3, T>* p_right, Vector<3, T>* p_out)
		{
			RVector_3_min((T*)(p_left), (T*)(p_right), (T*)p_out);
			return p_out;
		};
		template <typename T>
		inline static Vector<3, T>* mul(const Vector<3, T>* p_left, const T p_right, Vector<3, T>* p_out)
		{
			RVector_3_mul((T*)(p_left), p_right, (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Vector<3, T>* mul(const Vector<3, T>* p_left, const Vector<3, T>* p_right, Vector<3, T>* p_out)
		{
			RVector_3_mul((T*)(p_left), (T*)p_right, (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Vector<3, T>* inv(const Vector<3, T>* p_vec, Vector<3, T>* p_out)
		{
			RVector_3_inv((T*)(p_vec), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static float dot(const Vector<3, T>* p_left, const  Vector<3, T>* p_right)
		{
			return RVector_3_dot((T*)(p_left), (T*)(p_right));
		};
		template <typename T>
		inline static Vector<3, T>* project(const Vector<3, T>* p_vec, const Vector<3, T>* p_projectedOn, Vector<3, T>* p_out)
		{
			RVector_3_project((T*)(p_vec), (T*)(p_projectedOn), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Vector<3, T>* cross(const Vector<3, T>* p_left, const Vector<3, T>* p_right, Vector<3, T>* p_out)
		{
			RVector_3_cross((T*)(p_left), (T*)(p_right), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static float angle(const Vector<3, T>* p_begin, const Vector<3, T>* p_end)
		{
			return RVector_3_angle((T*)(p_begin), (T*)(p_end));
		};
		template <typename T>
		inline static float angle_normalized(const Vector<3, T>* p_begin, const Vector<3, T>* p_end)
		{
			return RVector_3_angle_normalized((T*)(p_begin), (T*)(p_end));
		};
		template <typename T>
		inline static short int angleSign(const Vector<3, T>* p_begin, const Vector<3, T>* p_end, const Vector<3, T>* p_referenceAxis)
		{
			return RVector_3_angleSign((T*)(p_begin), (T*)(p_end), (T*)(p_referenceAxis));
		};
		template <typename T>
		inline static Vector<3, T>* rotate(const Vector<3, T>* p_vec, const  Quaternion<T>* p_rotation, Vector<3, T>* p_out)
		{
			RVector_3_rotate((T*)(p_vec), (T*)(p_rotation), (T*)(p_out));
			return p_out;
		};

		template <typename T>
		inline static bool equals(const Vector<2, T>* p_left, const Vector<2, T>* p_right)
		{
			return RVector_2_equals((T*)(p_left), (T*)(p_right));
		};
		template <typename T>
		inline static bool equals(const Vector<3, T>* p_left, const Vector<3, T>* p_right)
		{
			return RVector_3_equals((T*)(p_left), (T*)(p_right));
		};

		template <typename T>
		inline static float length(const Vector<4, T>* p_vec)
		{
			return RVector_4_length((T*)(p_vec));
		};

		template <typename T>
		inline static Vector<4, T>* mul(const Vector<4, T>* p_left, const T p_right, Vector<4, T>* p_out)
		{
			RVector_4_mul((T*)(p_left), p_right, (T*)(p_out));
			return p_out;
		};



	};
}