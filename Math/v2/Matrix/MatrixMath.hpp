#pragma once

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Matrix/RMatrix.h"

namespace _MathV2
{
	struct MatrixM
	{
		template <typename T>
		inline static Matrix<3, 3, T> build(const Vector3<T>& p_col1, const Vector3<T>& p_col2, const Vector3<T>& p_col3)
		{
			Matrix<3, 3, T> l_return;
			RMatrix_3x3_buildFromColumn((T*)(&p_col1), (T*)(&p_col2), (T*)(&p_col3), l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static Vector3<T> right(const Matrix<3, 3, T>& p_mat)
		{
			return *(Vector3<T>*)(p_mat.Points[0]);
		};
		template <typename T>
		inline static Vector3<T> up(const Matrix<3, 3, T>& p_mat)
		{
			return *(Vector3<T>*)(p_mat.Points[1]);
		};

		template <typename T>
		inline static Vector3<T> forward(const Matrix<3, 3, T>& p_mat)
		{
			return *(Vector3<T>*)(p_mat.Points[2]);
		};


		template <typename T>
		inline static Vector4<T> right(const Matrix<4, 4, T>& p_mat)
		{
			return *(Vector4<T>*)(p_mat.Points[0]);
		};
		template <typename T>
		inline static Vector4<T> up(const Matrix<4, 4, T>& p_mat)
		{
			return *(Vector4<T>*)(p_mat.Points[1]);
		};
		template <typename T>
		inline static Vector4<T> forward(const Matrix<4, 4, T>& p_mat)
		{
			return *(Vector4<T>*)(p_mat.Points[2]);
		};
		template <typename T>
		inline static void setColumn(const Matrix<4, 4, T>& p_mat, int p_colIndex, const Vector<4, T>& p_col)
		{
			RMatrix_4x4_setColumn(p_mat.Points, p_colIndex, (T*)(&p_col));
		};

		template <typename T>
		inline static Matrix4x4<T> mul(const Matrix4x4<T>& p_left, const Matrix4x4<T>& p_right)
		{
			Matrix4x4<T> l_return;
			RMatrix_4x4_mul_4x4(p_left.Points, p_right.Points, l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static Vector4<T> mul(const Matrix4x4<T>& p_left, const Vector4<T>& p_right)
		{
			Vector4<T> l_return;
			RMatrix_4x4_mul_4(p_left.Points, (T*)&p_right, (T*)&l_return);
			return l_return;
		};

		template <typename T>
		inline static Matrix4x4<T> mul(const Matrix4x4<T>& p_left, T p_right)
		{
			Vector4<T> l_return;
			RMatrix_4x4_mul_1(p_left.Points, p_right, (T*)&l_return);
			return l_return;
		};

		template <typename T>
		inline static Matrix3x3<T> mul(const Matrix3x3<T>& p_left, const Matrix3x3<T>& p_right)
		{
			Matrix3x3<T> l_return;
			RMatrix_3x3_mul_3x3(p_left.Points, p_right.Points, l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static Vector3<T> mul(const Matrix3x3<T>& p_left, const Vector3<T>& p_right)
		{
			Vector3<T> l_return;
			RMatrix_3x3_mul_3(p_left.Points, (T*)(&p_right), (T*)(&l_return));
			return l_return;
		};

		template <typename T>
		inline static Matrix4x4<T> inv(const Matrix4x4<T>& p_mat)
		{
			Matrix4x4<T> l_return;
			RMatrix_4x4_inv(p_mat.Points, l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static void buildTranslationMatrix(Matrix4x4<T>& p_mat, Vector3<T>& p_translation)
		{
			RMatrix_4x4_buildTranslationMatrix(p_mat.Points, (T*)(&p_translation));
		}

		template <typename T>
		inline static void buildRotationMatrix(Matrix4x4<T>& p_mat, Matrix3x3<T>& p_axis)
		{
			RMatrix_4x4_buildRotationMatrix(p_mat.Points, p_axis.Points);
		};

		template <typename T>
		inline static void buildRotationMatrix(Matrix4x4<T>& p_mat, Vector3<T>& p_right, Vector3<T>& p_up, Vector3<T>& p_forward)
		{
			RMatrix_4x4_buildRotationMatrix(p_mat.Points, (T*)(&p_right), (T*)(&p_up), (T*)(&p_forward));
		};

		template <typename T>
		inline static void buildScaleMatrix(Matrix4x4<T>& p_mat, Vector3<T>& p_scale)
		{
			RMatrix_4x4_buildScaleMatrix(p_mat.Points, (T*)(&p_scale));
		};

		template <typename T>
		inline static Matrix4x4<T> buildTRS(Vector3<T>& p_position, Quaternion<T> p_quaternion, Vector3<T>& p_scale)
		{
			Matrix4x4<T> l_return;
			RMatrix_4x4_buildTRS((T*)(&p_position), (T*)(&p_quaternion), (T*)(&p_scale), l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static Matrix4x4<T> buildTRS(Vector3<T>& p_position, Vector3<T>& p_right, Vector3<T>& p_up, Vector3<T>& p_forward, Vector3<T>& p_scale)
		{
			Matrix4x4<T> l_return;
			RMatrix_4x4_buildTRS((T*)(&p_position), (T*)(&p_right), (T*)(&p_up), (T*)(&p_forward), (T*)(&p_scale), l_return.Points);
			return l_return;
		};

		template <typename T>
		inline static Vector4<T> getTranslation(Matrix4x4<T>& p_trs)
		{
			Vector4<T> l_return;
			RMatrix_4x4_getTranslation(p_trs.Points, (T*)(&l_return));
			return l_return;
		};

		template <typename T>
		inline static Vector4<T> getScale(Matrix4x4<T>& p_trs)
		{
			Vector4<T> l_return;
			RMatrix_4x4_getScale(p_trs.Points, (T*)(&l_return));
			return l_return;
		};
	};
}