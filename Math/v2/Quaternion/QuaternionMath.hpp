#pragma once

#include "Quaternion.hpp"
#include "RQuaternion.h"
#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"

#include "Functional/Equals/Equals.hpp"

namespace _MathV2
{
	 
	struct QuaternionM
	{
		static bool equals(const Quaternion<float>* p_left, const Quaternion<float>* p_right)
		{
			return _Core::Equals_float(&p_left->x, &p_right->x)
				&& _Core::Equals_float(&p_left->y, &p_right->y)
				&& _Core::Equals_float(&p_left->z, &p_right->z);
		};

		template <typename T>
		inline static Quaternion<T> build(const Vector<4, T>* p_quat)
		{
			return *(Quaternion<T>*) p_vec4;
		}
		template <typename T>
		inline static Quaternion<T> build(const Vector<3, T>* p_vector, const  T p_scalar)
		{
			return Quaternion<T>
			{
				    p_vector->x,
					p_vector->y,
					p_vector->z,
					p_scalar
			};
		};
		template <typename T>
		inline static Quaternion<T>* fromDirection(const Vector<3, T>* p_vect, Quaternion<T>* p_out)
		{
			RQuaternion_fromDirection((T*)(p_vect), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* fromAxis(const Matrix<3, 3, T>* p_axis, Quaternion<T>* p_out)
		{
			RQuaternion_fromAxis(p_axis->Points, (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* fromEulerAngle(const Vector<3, T>* p_eulerAngle, Quaternion<T>* p_out)
		{
			RQuaternion_fromEulerAngle((T*)(p_eulerAngle), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* fromTo(const Vector<3, T>* p_from, const  Vector<3, T>* p_to, Quaternion<T>* p_out)
		{
			RQuaternion_fromTo((T*)(p_from), (T*)(p_to), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Vector<3, T>* vector(const Quaternion<T>* p_quat, Vector<3, T>* p_out)
		{
			*p_out = *(Vector<3, T>*)(p_quat);
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* conjugate(const Quaternion<T>* p_quat, Quaternion<T>* p_out)
		{
			RQuaternion_conjugate((T*)(p_quat), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* normalize(const Quaternion<T>* p_quat, Quaternion<T>* p_out)
		{
			RQuaternion_normalize((T*)(p_quat), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* mul(const Quaternion<T>* p_left, const  Quaternion<T>* p_right, Quaternion<T>* p_out)
		{
			RQuaternion_mul((T*)(p_left), (T*)(p_right), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* cross(const Quaternion<T>* p_left, const  Quaternion<T>* p_right, Quaternion<T>* p_out)
		{
			RQuaternion_cross((T*)(p_left), (T*)(p_right), (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Quaternion<T>* rotateAround(const Vector3<T>* p_axis, const  T p_angle, Quaternion<T>* p_out)
		{
			RQuaternion_rotateAround((T*)(p_axis), p_angle, (T*)(p_out));
			return p_out;
		};
		template <typename T>
		inline static Matrix3x3<T>* extractAxis(const Quaternion<T>* p_quat, Matrix3x3<T>* p_out)
		{
			RQuaternion_extractAxis((T*)(p_quat), p_out->Points);
			return p_out;
		};
	};

}