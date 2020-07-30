#pragma once

#include "Quaternion.hpp"

namespace _MathV2
{
	struct QuaternionM
	{
		template <typename T>
		inline static Quaternion<T> build(Vector<4, T>& p_quat)
		{
			return *(Quaternion<T>*) & p_vec4;
		}
		template <typename T>
		inline static Quaternion<T> build(Vector<3, T>& p_vector, T p_scalar)
		{
			return Quaternion<T>
			{
				p_vector.x,
					p_vector.y,
					p_vector.z,
					p_scalar
			};
		};
		template <typename T>
		inline static Quaternion<T> fromDirection(Vector<3, T>& p_vect)
		{
			Quaternion<T> l_out;
			RQuaternion_fromDirection((T*)(&p_vect), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Quaternion<T> fromAxis(Matrix<3, 3, T>& p_axis)
		{
			Quaternion<T> l_out;
			RQuaternion_fromAxis(p_axis.Points, (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Quaternion<T> fromEulerAngle(Vector<3, T>& p_eulerAngle)
		{
			Quaternion<T> l_out;
			RQuaternion_fromEulerAngle((T*)(&p_eulerAngle), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Quaternion<T> fromTo(Vector<3, T>& p_from, Vector<3, T>& p_to)
		{
			Quaternion<T> l_out;
			RQuaternion_fromTo((T*)(&p_from), (T*)(&p_to), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Vector<3, T> vector(Quaternion<T>& p_quat)
		{
			return *(Vector<3, T>*)(&p_quat);
		};
		template <typename T>
		inline static Quaternion<T> conjugate(Quaternion<T>& p_quat)
		{
			Quaternion<T> l_out;
			RQuaternion_conjugate((T*)(&p_quat), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Quaternion<T> normalize(Quaternion<T>& p_quat)
		{
			Quaternion<T> l_out;
			RQuaternion_normalize((T*)(&p_quat), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Quaternion<T> mul(Quaternion<T>& p_left, Quaternion<T>& p_right)
		{
			Quaternion<T> l_out;
			RQuaternion_mul((T*)(&p_left), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Quaternion<T> cross(Quaternion<T>& p_left, Quaternion<T>& p_right)
		{
			Quaternion<T> l_out;
			RQuaternion_cross((T*)(&p_left), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Quaternion<T> rotateAround(Vector3<T>& p_axis, T p_angle)
		{
			Quaternion<T> l_out;
			RQuaternion_rotateAround((T*)(&p_axis), p_angle, (T*)(&l_out));
			return l_out;
		};
		template <typename T>
		inline static Matrix3x3<float> extractAxis(Quaternion<T>& p_quat)
		{
			Matrix3x3<float> l_out;
			RQuaternion_extractAxis((T*)(&p_quat), l_out.Points);
			return l_out;
		};
	};

	

}