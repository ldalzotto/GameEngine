#pragma once

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Quaternion/Quaternion.hpp"
extern "C"
{
#include "v2/_source/MatrixC.h"
}

namespace _MathV2
{
	struct MatrixM
	{
		inline static Matrix<3, 3, float> build(const Vector3<float>* p_col1, const Vector3<float>* p_col2, const Vector3<float>* p_col3)
		{
			Matrix<3, 3, float> l_return;
			l_return.Right = *p_col1;
			l_return.Up = *p_col2;
			l_return.Forward = *p_col3;
			return l_return;
		};

		inline static Matrix<4, 4, float> build(const Vector4<float>* p_col1, const Vector4<float>* p_col2, const Vector4<float>* p_col3, const Vector4<float>* p_col4)
		{
			Matrix<4, 4, float> l_return;
			l_return.Right = *p_col1;
			l_return.Up = *p_col2;
			l_return.Forward = *p_col3;
			l_return.Col3 = *p_col4;
			return l_return;
		};

		inline static Matrix<4, 4, float>* mul(const Matrix<4, 4, float>* p_left, const Matrix<4, 4, float>* p_right, Matrix<4, 4, float>* p_out)
		{
			Mat_Mul_M4F_M4F((const MATRIX4F_PTR)p_left, (const MATRIX4F_PTR)p_right, (MATRIX4F_PTR)p_out);
			return p_out;
		};

		inline static Vector<4, float>* mul(const Matrix<4, 4, float>* p_left, const Vector<4, float>* p_right, Vector<4, float>* p_out)
		{
			Mat_Mul_M4F_V4F((const MATRIX4F_PTR)p_left, (const VECTOR4F_PTR)p_right, (VECTOR4F_PTR)p_out);
			return p_out;
		};

		inline static Vector<3, float>* mul(const Matrix<3, 3, float>* p_left, const Vector<3, float>* p_right, Vector<3, float>* p_out)
		{
			Mat_Mul_M3F_V3F((const MATRIX3F_PTR)p_left, (const VECTOR3F_PTR)p_right, (VECTOR3F_PTR)p_out);
			return p_out;
		};

		inline static Matrix4x4<float>* inv(const Matrix4x4<float>* p_mat, Matrix4x4<float>* p_out)
		{
			Mat_Inv_M4F((const MATRIX4F_PTR)p_mat, (MATRIX4F_PTR)p_out);
			return p_out;
		};

		inline static Matrix4x4<float>* buildTranslationMatrix(Vector3<float>* p_translation, Matrix4x4<float>* p_out)
		{
			Mat_Translation_M4F((const VECTOR3F_PTR)p_translation, (MATRIX4F_PTR)p_out);
			return p_out;
		}

		inline static Matrix4x4<float>* buildRotationMatrix(const Matrix3x3<float>* p_axis, Matrix4x4<float>* p_out)
		{
			Mat_RotationAxis_M4F((const MATRIX3F_PTR)p_axis, (MATRIX4F_PTR)p_out);
			return p_out;
		}

		inline static Matrix4x4<float>* buildScaleMatrix(const Vector3<float>* p_scale, Matrix4x4<float>* p_out)
		{
			Mat_Scale_M4F((const VECTOR3F_PTR)p_scale, (MATRIX4F_PTR)p_out);
			return p_out;
		};

		inline static Matrix4x4<float>* buildTRS(Vector3<float>* p_position, Quaternion<float>* p_quaternion, Vector3<float>* p_scale, Matrix4x4<float>* p_out)
		{
			Mat_TRS_Quat_M4F((const VECTOR3F_PTR)p_position, (const QUATERNION4F_PTR)p_quaternion, (const VECTOR3F_PTR)p_scale, (MATRIX4F_PTR)p_out);
			return p_out;
		};

		inline static Matrix4x4<float>* buildTRS(Vector3<float>* p_position, Matrix3x3<float>* p_axis, Vector3<float>* p_scale, Matrix4x4<float>* p_out)
		{
			Mat_TRS_Axis_M4F((const VECTOR3F_PTR)p_position, (const MATRIX3F_PTR)p_axis, (const VECTOR3F_PTR)p_scale, (MATRIX4F_PTR)p_out);
			return p_out;
		};

		template <typename T>
		inline static Vector4<T> getTranslation(const Matrix4x4<T>* p_trs)
		{
			Vector4<T> l_return;
			Mat_GetTranslation_M4F((const MATRIX4F_PTR)p_trs, (VECTOR4F_PTR)(&l_return));
			return l_return;
		};

		template <typename T>
		inline static Vector4<T> getScale(Matrix4x4<T>* p_trs)
		{
			Vector4<T> l_return;
			Mat_GetScale_M4F((const MATRIX4F_PTR)p_trs, (VECTOR4F_PTR)&l_return);
			return l_return;
		};

		inline static Matrix4x4<float>* perspective(const float p_fov, const float p_aspect, const float p_near, const float p_far, Matrix4x4<float>* p_out)
		{
			Mat_Perspective_M4F(p_fov, p_aspect, p_near, p_far, (MATRIX4F_PTR)p_out);
			return p_out;
		};

		inline static Matrix3x3<float>* lookAt_rotation(const Vector3<float>* p_origin, const Vector3<float>* p_target, const Vector3<float>* p_up, Matrix3x3<float>* p_out)
		{
			Mat_LookAtRotation_F((const VECTOR3F_PTR)p_origin, (const VECTOR3F_PTR)p_target, (const VECTOR3F_PTR)p_up, (MATRIX3F_PTR)p_out);
			return p_out;
		};

		template <typename T>
		inline static Vector4<T>* mul_homogeneous(const Matrix4x4<T>* p_projectionmatrix, const Vector4<T>* p_pos, Vector4<T>* p_out)
		{
			Mat_Mul_M4F_V4F_Homogeneous((const MATRIX4F_PTR)p_projectionmatrix, (const VECTOR4F_PTR)p_pos, (VECTOR4F_PTR)p_out);
			return p_out;
		};
	};
}