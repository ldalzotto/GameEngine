#pragma once

#include "v2/Matrix/Matrix.hpp"
#include "v2/Matrix/Matrix.h"

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
		static Vector3<T> right(const Matrix<3, 3, T>& p_mat)
		{
			return *(Vector3<T>*)(p_mat.Points[0]);
		};
		template <typename T>
		static Vector3<T> up(const Matrix<3, 3, T>& p_mat)
		{
			return *(Vector3<T>*)(p_mat.Points[1]);
		};
		template <typename T>
		static Vector3<T> forward(const Matrix<3, 3, T>& p_mat)
		{
			return *(Vector3<T>*)(p_mat.Points[2]);
		};
	};
}