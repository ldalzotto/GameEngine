#pragma once

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Quaternion/Quaternion.hpp"
#include "v2/Matrix/RMatrix.h"

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

		template <typename T, int C1, int L1, int C2, int L2>
		inline static Matrix<C2, L1, T >* mul(const Matrix<C1, L1, T>* p_left, const Matrix<C2, L2, T >* p_right, Matrix<C2, L1, T >* p_out)
		{
			RMatrix_mul_specification((const T*)p_left->Points, C1, L1, (const T*)p_right, C2, L2, (T*)p_out);
			return p_out;
		};

		template <typename T, int C1, int L1>
		inline static Vector<L1, T>* mul(const Matrix<C1, L1, T>* p_left, const Vector<L1, T>* p_right, Vector<L1, T>* p_out)
		{
			RMatrix_mul_specification((const T*)p_left->Points, C1, L1, (const T*)p_right, 1, L1, (T*)p_out);
			return p_out;
		};

		template <typename T>
		inline static Matrix4x4<T>* inv(const Matrix4x4<T>* p_mat, Matrix4x4<T>* p_out)
		{
			RMatrix_4x4_inv(p_mat->Points, p_out->Points);
			return p_out;
		};

		template <typename T>
		inline static Matrix4x4<T>* buildTranslationMatrix(Vector3<T>* p_translation, Matrix4x4<T>* p_out)
		{
			*p_out = Matrix4x4f_Identity;
			RMatrix_4x4_buildTranslationMatrix(p_out->Points, (T*)(p_translation));
			return p_out;
		}

		template <typename T>
		inline static Matrix4x4<T>* buildRotationMatrix(const Matrix3x3<T>* p_axis, Matrix4x4<T>* p_out)
		{
			RMatrix_4x4_buildRotationMatrix(p_out->Points, p_axis->Points);
			return p_out;
		}

		template <typename T>
		inline static Matrix3x3<T>* buildAxisMatrix(Vector3<T>* p_right, Vector3<T>* p_up, Vector3<T>* p_forward, Matrix3x3<T>* p_out)
		{
			RMatrix_3x3_buildFromColumn((T*)(p_right), (T*)(p_up), (T*)(p_forward), p_out->Points);
			return p_out;
		};

		template <typename T>
		inline static Matrix4x4<T>* buildScaleMatrix(const Vector3<T>* p_scale, Matrix4x4<T>* p_out)
		{
			*p_out = Matrix4x4f_Identity;
			RMatrix_4x4_buildScaleMatrix(p_out->Points, (T*)(&p_scale));
			return p_out;
		};

		template <typename T>
		inline static Matrix4x4<T>* buildTRS(Vector3<T>* p_position, Quaternion<T>* p_quaternion, Vector3<T>* p_scale, Matrix4x4<T>* p_out)
		{
			RMatrix_4x4_buildTRS((T*)(p_position), (T*)(p_quaternion), (T*)(p_scale), p_out->Points);
			return p_out;
		};

		template <typename T>
		inline static Matrix4x4<T>* buildTRS(Vector3<T>* p_position, Matrix3x3<T>* p_axis, Vector3<T>* p_scale, Matrix4x4<T>* p_out)
		{
			RMatrix_4x4_buildTRS((T*)(p_position), p_axis->Points, (T*)(p_scale), p_out->Points);
			return p_out;
		};

		template <typename T>
		inline static Vector4<T> getTranslation(Matrix4x4<T>* p_trs)
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
		inline static Vector4<T>* clipSpaceMul(const Matrix4x4<T>* p_projectionmatrix, const Vector4<T>* p_pos, Vector4<T>* p_out)
		{
			RMatrix_4x4_clipSpaceMul(p_projectionmatrix->Points, (T*)(p_pos), (T*)(p_out));
			return p_out;
		};
	};
}