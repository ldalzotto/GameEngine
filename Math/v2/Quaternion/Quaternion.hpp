#pragma once

#include "Functional/Equals/Equals.hpp"

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "RQuaternion.h"

namespace _MathV2
{
	template <typename T>
	struct Quaternion
	{
		T x;
		T y;
		T z;
		T w = 1;

		inline static Quaternion<T> build(Vector<4, T>& p_quat)
		{
			return *(Quaternion<T>*) & p_vec4;
		}

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

		inline static Quaternion<T> fromDirection(Vector<3, T>& p_vect)
		{
			Quaternion<T> l_out;
			RQuaternion_fromDirection((T*)(&p_vect), (T*)(&l_out));
			return l_out;
		};

		inline static Quaternion<T> fromAxis(Matrix<3, 3, T>& p_axis)
		{
			Quaternion<T> l_out;
			RQuaternion_fromAxis(p_axis.Points, (T*)(&l_out));
			return l_out;
		};

		inline static Quaternion<T> fromEulerAngle(Vector<3, T>& p_eulerAngle)
		{
			Quaternion<T> l_out;
			RQuaternion_fromEulerAngle((T*)(&p_eulerAngle), (T*)(&l_out));
			return l_out;
		};

		inline static Quaternion<T> fromTo(Vector<3, T>& p_from, Vector<3, T>& p_to)
		{
			Quaternion<T> l_out;
			RQuaternion_fromTo((T*)(&p_from), (T*)(&p_to), (T*)(&l_out));
			return l_out;
		};

		inline Vector<3, T> vector()
		{
			return *(Vector<3, T>*)this;
		};

		inline Quaternion<T> conjugate()
		{
			Quaternion<T> l_out;
			RQuaternion_conjugate((T*)(this), (T*)(&l_out));
			return l_out;
		};

		inline Quaternion<T> normalize()
		{
			Quaternion<T> l_out;
			RQuaternion_normalize((T*)(this), (T*)(&l_out));
			return l_out;
		};

		inline Quaternion<T> mul(Quaternion<T>& p_right)
		{
			Quaternion<T> l_out;
			RQuaternion_mul((T*)(this), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};

		inline Quaternion<T> cross(Quaternion<T>& p_right)
		{
			Quaternion<T> l_out;
			RQuaternion_cross((T*)(this), (T*)(&p_right), (T*)(&l_out));
			return l_out;
		};

		static Quaternion<T> rotateAround(Vector3<T>& p_axis, T p_angle)
		{
			Quaternion<T> l_out;
			RQuaternion_rotateAround((T*)(&p_axis), p_angle, (T*)(&l_out));
			return l_out;
		};

		inline Matrix3x3<float> extractAxis()
		{
			Matrix3x3<float> l_out;
			RQuaternion_extractAxis((T*)(this), l_out.Points);
			return l_out;
		};

	};

	inline static const Quaternion<float> Quaternionf_Identity = { 0.0f, 0.0f, 0.0f, 1.0f };

	bool Quaternion_Equals(Quaternion<float>& p_left, Quaternion<float>& p_right);
}