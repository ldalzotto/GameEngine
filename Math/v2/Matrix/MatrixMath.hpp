#pragma once

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Quaternion/Quaternion.hpp"
#include "v2/Matrix/RMatrix.h"
extern "C"
{
#include "v2/_source/MatrixC.h"
}

namespace _MathV2
{
	struct MatrixM
	{
		template <typename T>
		inline static Matrix<3, 3, T> build(const Vector3<T>* p_col1, const Vector3<T>* p_col2, const Vector3<T>* p_col3)
		{
			Matrix<3, 3, T> l_return;
			RMatrix_3x3_buildFromColumn((T*)(p_col1), (T*)(p_col2), (T*)(p_col3), l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static Matrix<4, 4, T> build(const Vector4<T>* p_col1, const Vector4<T>* p_col2, const Vector4<T>* p_col3, const Vector4<T>* p_col4)
		{
			Matrix<4, 4, T> l_return;
			RMatrix_4x4_buildFromColumn((T*)(p_col1), (T*)(p_col2), (T*)(p_col3), (T*)(p_col4), l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static Vector3<T> right(const Matrix<3, 3, T>* p_mat)
		{
			return *(Vector3<T>*)(p_mat->Points[0]);
		};
		template <typename T>
		inline static Vector3<T> up(const Matrix<3, 3, T>* p_mat)
		{
			return *(Vector3<T>*)(p_mat->Points[1]);
		};

		template <typename T>
		inline static Vector3<T> forward(const Matrix<3, 3, T>* p_mat)
		{
			return *(Vector3<T>*)(p_mat->Points[2]);
		};


		template <typename T>
		inline static Vector4<T> right(const Matrix<4, 4, T>* p_mat)
		{
			return *(Vector4<T>*)(p_mat->Points[0]);
		};
		template <typename T>
		inline static Vector4<T> up(const Matrix<4, 4, T>* p_mat)
		{
			return *(Vector4<T>*)(p_mat->Points[1]);
		};
		template <typename T>
		inline static Vector4<T> forward(const Matrix<4, 4, T>* p_mat)
		{
			return *(Vector4<T>*)(p_mat->Points[2]);
		};
		template <typename T>
		inline static void setColumn(const Matrix<4, 4, T>* p_mat, int p_colIndex, const Vector<4, T>* p_col)
		{
			RMatrix_4x4_setColumn(p_mat->Points, p_colIndex, (T*)(p_col), 4);
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
			RMatrix_4x4_buildTRS((float*)(p_position), (float*)(p_quaternion), (float*)(p_scale), p_out->Points);
			return p_out;
		};

		inline static Matrix4x4<float>* buildTRS(Vector3<float>* p_position, Matrix3x3<float>* p_axis, Vector3<float>* p_scale, Matrix4x4<float>* p_out)
		{
			RMatrix_4x4_buildTRS((float*)(p_position), p_axis->Points, (float *)(p_scale), p_out->Points);
			return p_out;
		};

		template <typename T>
		inline static Vector4<T> getTranslation(const Matrix4x4<T>* p_trs)
		{
			Vector4<T> l_return;
			RMatrix_4x4_getTranslation(p_trs->Points, (T*)(&l_return));
			return l_return;
		};

		template <typename T>
		inline static Vector4<T> getScale(Matrix4x4<T>* p_trs)
		{
			Vector4<T> l_return;
			RMatrix_4x4_getScale(p_trs->Points, (T*)(&l_return));
			return l_return;
		};

		template <typename T>
		inline static Matrix4x4<T>* perspective(const float p_fov, const float p_aspect, const float p_near, const float p_far, Matrix4x4<T>* p_out)
		{
			RMatrix_4x4_perspective(p_fov, p_aspect, p_near, p_far, p_out->Points);
			return p_out;
		};

		template <typename T>
		inline static Matrix3x3<T>* lookAt_rotation(const Vector3<T>* p_origin, const Vector3<T>* p_target, const Vector3<T>* p_up, Matrix3x3<T>* p_out)
		{
			RMatrix_3x3_lookAt_rotation((T*)(p_origin), (T*)(p_target), (T*)(p_up), p_out->Points);
			return p_out;
		};

		template <typename T>
		inline static Vector4<T>* mul_homogeneous(const Matrix4x4<T>* p_projectionmatrix, const Vector4<T>* p_pos, Vector4<T>* p_out)
		{
			RMatrix_4x4_mul_homogeneous(p_projectionmatrix->Points, (T*)(p_pos), (T*)(p_out));
			return p_out;
		};
	};
}