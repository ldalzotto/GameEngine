#pragma once

extern "C"
{
#include "v2/_interface/QuaternionC_def.h"
#include "v2/_source/VectorC.h"
}

#include "Vector.hpp"

namespace _MathV2
{
	struct VectorM
	{
		inline static Vector<2, float> cast_if(const Vector<2, int>* p_vec)
		{
			return Vector<2, float>{(float)p_vec->x, (float)p_vec->y};
		}

		template <typename T>
		inline static Vector<2, T>* cast2(Vector<4, T>* p_vec)
		{
			return (Vector<2, T>*)p_vec;
		};

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
		
		inline static float distance(const Vector<3, float>* p_start, const Vector<3, float>* p_end)
		{
			return Vec_Distance_3f((const VECTOR3F_PTR)p_start, (const VECTOR3F_PTR)p_end);
		};

		inline static Vector<3, float>* normalize(const Vector<3, float>* p_vec, Vector<3, float>* p_out)
		{
			Vec_Normalize_3f((const VECTOR3F_PTR)p_vec, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static Vector<3, float>* add(const Vector<3, float>* p_left, const Vector<3, float>* p_right, Vector<3, float>* p_out)
		{
			Vec_Add_3f_3f((const VECTOR3F_PTR)p_left, (const VECTOR3F_PTR)p_right, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static Vector<4, float>* add(const Vector<4, float>* p_left, const Vector<4, float>* p_right, Vector<4, float>* p_out)
		{
			Vec_Add_4f_4f((const VECTOR4F_PTR)p_left, (const VECTOR4F_PTR)p_right, (VECTOR4F_PTR)p_out);
			return p_out;
		};

		inline static Vector<2, double>* min(const Vector<2, double>* p_left, const Vector<2, double>* p_right, Vector<2, double>* p_out)
		{
			Vec_Min_2d_2d((const VECTOR2D_PTR)p_left, (const VECTOR2D_PTR)p_right, (VECTOR2D_PTR)p_out);
			return p_out;
		};

		inline static Vector<3, float>* min(const Vector<3, float>* p_left, const Vector<3, float>* p_right, Vector<3, float>* p_out)
		{
			Vec_Min_3f_3f((const VECTOR3F_PTR)p_left, (const VECTOR3F_PTR)p_right, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static Vector<4, float>* min(const Vector<4, float>* p_left, const Vector<4, float>* p_right, Vector<4, float>* p_out)
		{
			Vec_Min_4f_4f((const VECTOR4F_PTR)p_left, (const VECTOR4F_PTR)p_right, (VECTOR4F_PTR)p_out);
			return p_out;
		};

		inline static Vector<2, float>* mul(const Vector<2, float>* p_left, const float p_right, Vector<2, float>* p_out)
		{
			Vec_Mul_2f_1f((const VECTOR2F_PTR)p_left, p_right, (VECTOR2F_PTR)p_out);
			return p_out;
		};

		inline static Vector<3, float>* mul(const Vector<3, float>* p_left, const float p_right, Vector<3, float>* p_out)
		{
			Vec_Mul_3f_1f((const VECTOR3F_PTR)(p_left), p_right, (VECTOR3F_PTR)(p_out));
			return p_out;
		};

		inline static Vector<3, float>* mul(const Vector<3, float>* p_left, const Vector<3, float>* p_right, Vector<3, float>* p_out)
		{
			Vec_Mul_3f_3f((const VECTOR3F_PTR)(p_left), (const VECTOR3F_PTR)p_right, (VECTOR3F_PTR)(p_out));
			return p_out;
		};

		inline static Vector<3, float>* inv(const Vector<3, float>* p_vec, Vector<3, float>* p_out)
		{
			Vec_Inv_3f((const VECTOR3F_PTR)p_vec, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static float dot(const Vector<3, float>* p_left, const  Vector<3, float>* p_right)
		{
			return Vec_Dot_3f((const VECTOR3F_PTR)p_left, (const VECTOR3F_PTR)p_right);
		};

		
		inline static Vector<3, float>* project(const Vector<3, float>* p_vec, const Vector<3, float>* p_projectedOn, Vector<3, float>* p_out)
		{
			Vec_Project_3f((const VECTOR3F_PTR)p_vec, (const VECTOR3F_PTR)p_projectedOn, (VECTOR3F_PTR) p_out);
			return p_out;
		};

		inline static Vector<3, float>* cross(const Vector<3, float>* p_left, const Vector<3, float>* p_right, Vector<3, float>* p_out)
		{
			Vec_Cross_3f((const VECTOR3F_PTR)p_left, (const VECTOR3F_PTR)p_right, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static float angle(const Vector<3, float>* p_begin, const Vector<3, float>* p_end)
		{
			return Vec_Angle_3f((const VECTOR3F_PTR)p_begin, (const VECTOR3F_PTR)p_end);
		};
		

		inline static float angle_normalized(const Vector<3, float>* p_begin, const Vector<3, float>* p_end)
		{
			return Vec_Angle_Normalized_3f((const VECTOR3F_PTR)p_begin, (const VECTOR3F_PTR)p_end);
		};
		

		inline static short int angleSign(const Vector<3, float>* p_begin, const Vector<3, float>* p_end, const Vector<3, float>* p_referenceAxis)
		{
			return Vec_AngleSign_3f((const VECTOR3F_PTR)p_begin, (const VECTOR3F_PTR)p_end, (VECTOR3F_PTR)p_referenceAxis);
		};

		inline static Vector<3, float>* rotate(const Vector<3, float>* p_vec, const QUATERNION4F_PTR p_rotation, Vector<3, float>* p_out)
		{
			Vec_Rotate_3f((const VECTOR3F_PTR)p_vec, p_rotation, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static bool equals(const Vector<2, float>* p_left, const Vector<2, float>* p_right)
		{
			return Vec_Equals_2f((const VECTOR2F_PTR)p_left, (const VECTOR2F_PTR)p_right);
		};

		inline static bool equals(const Vector<2, double>* p_left, const Vector<2, double>* p_right)
		{
			return Vec_Equals_2d((const VECTOR2D_PTR)p_left, (const VECTOR2D_PTR)p_right);
		};

		inline static bool equals(const Vector<3, float>* p_left, const Vector<3, float>* p_right)
		{
			return Vec_Equals_3f((const VECTOR3F_PTR)p_left, (const VECTOR3F_PTR)p_right);
		};

		inline static float length(const Vector<3, float>* p_vec)
		{
			return Vec_Length_3f((const VECTOR3F_PTR)p_vec);
		};

		inline static float length(const Vector<4, float>* p_vec)
		{
			return Vec_Length_4f((const VECTOR4F_PTR) p_vec);
		};

		inline static Vector<4, float>* mul(const Vector<4, float>* p_left, const float p_right, Vector<4, float>* p_out)
		{
			Vec_Mul_4f_1f((const VECTOR4F_PTR)p_left, p_right, (VECTOR4F_PTR)p_out);
			return p_out;
		};
	};
}