#pragma once

#include "Quaternion.hpp"
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
		inline static bool equals(const Quaternion<float>* p_left, const Quaternion<float>* p_right)
		{
			return Quat_Equals((const QUATERNION4F_PTR)p_left, (const QUATERNION4F_PTR)p_right);
		};

		inline static Quaternion<float> build(const Vector<4, float>* p_quat)
		{
			Quaternion<float> l_return;
			Quat_Build_4f((const VECTOR4F_PTR)p_quat, (QUATERNION4F_PTR)&l_return);
			return l_return;
		}

		inline static Quaternion<float> build(const Vector<3, float>* p_vector, const  float p_scalar)
		{
			Quaternion<float> l_return;
			Quat_Build_3f_1f((const VECTOR3F_PTR)p_vector, p_scalar, (QUATERNION4F_PTR)&l_return);
			return l_return;
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