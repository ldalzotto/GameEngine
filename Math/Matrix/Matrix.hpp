#pragma once

namespace _Math
{
	/* Matrices are column driven */

	template<int C, int L, typename T>
	struct Matrix { };

	template<typename T>
	struct Matrix<3, 3, T>
	{
		T Points[3][3];
	};

	template<typename T>
	using Matrix3x3 = Matrix<3, 3, T>;
}