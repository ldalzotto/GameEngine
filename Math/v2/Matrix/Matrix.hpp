#pragma once

#include "Matrix.h"

namespace _MathV2
{
	/* Matrices are column driven */

	template<int C, int L, typename T>
	struct Matrix { };

	template<typename T>
	struct Matrix<3, 3, T>
	{
		T Points[3][3];

		static Matrix<3, 3, T> build(Vector3<T>& p_col1, Vector3<T>& p_col2, Vector3<T>& p_col3)
		{
			Matrix<3, 3, T> l_return;
			RMatrix_3x3_buildFromColumn((T*)(&p_col1), (T*)(&p_col2), (T*)(&p_col3), l_return.Points);
			return l_return;
		};

		T* right()
		{
			return this->Points[0];
		};
		T* up()
		{
			return this->Points[1];
		};
		T* forward()
		{
			return this->Points[2];
		};
	};

	template<typename T>
	using Matrix3x3 = Matrix<3, 3, T>;
}