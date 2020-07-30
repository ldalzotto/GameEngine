#pragma once

#include "v2/Vector/Vector.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "v2/Matrix/RMatrix.h"

namespace _MathV2
{
	struct MatrixM
	{
		template <typename T>
		static Matrix<3, 3, T> build(const Vector3<T>& p_col1, const Vector3<T>& p_col2, const Vector3<T>& p_col3)
		{
			Matrix<3, 3, T> l_return;
			RMatrix_3x3_buildFromColumn((T*)(&p_col1), (T*)(&p_col2), (T*)(&p_col3), l_return.Points);
			return l_return;
		};

		template <typename T>
		static Vector3<T>* right_ref(const Matrix<3, 3, T>& p_mat)
		{
			return (Vector3<T>*)(p_mat.Points[0]);
		};
		template <typename T>
		static Vector3<T>* up_ref(const Matrix<3, 3, T>& p_mat)
		{
			return (Vector3<T>*)(p_mat.Points[1]);
		};

		template <typename T>
		static Vector3<T>* forward_ref(const Matrix<3, 3, T>& p_mat)
		{
			return (Vector3<T>*)(p_mat.Points[2]);
		};


		template <typename T>
		static Vector4<T>* right_ref(const Matrix<4, 4, T>& p_mat)
		{
			return (Vector4<T>*)(p_mat.Points[0]);
		};
		template <typename T>
		static Vector4<T>* up_ref(const Matrix<4, 4, T>& p_mat)
		{
			return (Vector4<T>*)(p_mat.Points[1]);
		};
		template <typename T>
		static Vector4<T>* forward_ref(const Matrix<4, 4, T>& p_mat)
		{
			return (Vector4<T>*)(p_mat.Points[2]);
		};

		template <typename T>
		static Matrix4x4<T> mul(const Matrix4x4<T>& p_left, const Matrix4x4<T>& p_right)
		{
			Matrix4x4<T> l_return;
			RMatrix_4x4_mul_4x4(p_left.Points, p_right.Points, l_return.Points);
			return l_return;
		};

		template <typename T>
		static Vector4<T> mul(const Matrix4x4<T>& p_left, const Vector4<T>& p_right)
		{
			Vector4<T> l_return;
			RMatrix_4x4_mul_4(p_left.Points, (T*)&p_right, (T*)&l_return);
			return l_return;
		};

		template <typename T>
		static Matrix3x3<T> mul(const Matrix3x3<T>& p_left, const Matrix3x3<T>& p_right)
		{
			Matrix3x3<T> l_return;
			RMatrix_3x3_mul_3x3(p_left.Points, p_right.Points, l_return.Points);
			return l_return;
		};
	};
}