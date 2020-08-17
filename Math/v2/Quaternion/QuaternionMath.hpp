#pragma once

#include "Quaternion.hpp"
#include "RQuaternion.h"
#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"

extern "C"
{
#include "Functional/Equals/Equals.h"
#include "v2/_source/QuaternionC.h"
}

namespace _MathV2
{

	struct QuaternionM
	{
		static bool equals(const Quaternion<float>* p_left, const Quaternion<float>* p_right)
		{
			return Equals_float(&p_left->x, &p_right->x)
				&& Equals_float(&p_left->y, &p_right->y)
				&& Equals_float(&p_left->z, &p_right->z);
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

		inline static Quaternion<float>* fromDirection(const Vector<3, float>* p_vect, Quaternion<float>* p_out)
		{
			Quat_FromDirection((const VECTOR3F_PTR)p_vect, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Quaternion<float>* fromAxis(const Matrix<3, 3, float>* p_axis, Quaternion<float>* p_out)
		{
			Quat_FromAxis(p_axis->Points, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Quaternion<float>* fromEulerAngle(const Vector<3, float>* p_eulerAngle, Quaternion<float>* p_out)
		{
			Quat_FromEulerAngle((const VECTOR3F_PTR)p_eulerAngle, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Quaternion<float>* fromTo(const Vector<3, float>* p_from, const  Vector<3, float>* p_to, Quaternion<float>* p_out)
		{
			Quat_FromTo((const VECTOR3F_PTR)p_from, (const VECTOR3F_PTR)p_to, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Vector<3, float>* vector(const Quaternion<float>* p_quat, Vector<3, float>* p_out)
		{
			*p_out = *(Vector<3, float>*)(p_quat);
			return p_out;
		};

		inline static Quaternion<float>* conjugate(const Quaternion<float>* p_quat, Quaternion<float>* p_out)
		{
			Quat_conjugate((const QUATERNION4F_PTR)p_quat, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Quaternion<float>* normalize(const Quaternion<float>* p_quat, Quaternion<float>* p_out)
		{
			Quat_Normalize((const QUATERNION4F_PTR)p_quat, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Quaternion<float>* mul(const Quaternion<float>* p_left, const  Quaternion<float>* p_right, Quaternion<float>* p_out)
		{
			Quat_Mul((const QUATERNION4F_PTR)p_left, (const QUATERNION4F_PTR)p_right, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Quaternion<float>* cross(const Quaternion<float>* p_left, const  Quaternion<float>* p_right, Quaternion<float>* p_out)
		{
			Quat_Cross((const QUATERNION4F_PTR)p_left, (const QUATERNION4F_PTR)p_right, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Quaternion<float>* rotateAround(const Vector3<float>* p_axis, const  float p_angle, Quaternion<float>* p_out)
		{
			Quat_RotateAround((const VECTOR3F_PTR)p_axis, p_angle, (QUATERNION4F_PTR)p_out);
			return p_out;
		};

		inline static Matrix3x3<float>* extractAxis(const Quaternion<float>* p_quat, Matrix3x3<float>* p_out)
		{
			Quat_ExtractAxis((const QUATERNION4F_PTR)p_quat, p_out->Points);
			return p_out;
		};
	};

}