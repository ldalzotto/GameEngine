#pragma once

#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	/* Matrices are column driven */

	template<int C, int L, typename T>
	struct Matrix { };

	template<typename T>
	struct Matrix<3, 3, T>
	{
		union
		{
			T Points[3][3];
			struct { Vector<3, T> Right; Vector<3, T> Up; Vector<3, T> Forward; };
		};
	};

	template<typename T>
	using Matrix3x3 = Matrix<3, 3, T>;

	extern const Matrix3x3<float> Matrix3x3f_Identity;

	/**
Convention
	  - column major matrix

	  [RIGHT.x   UP.x    FORWARD.x    POS.x ]
	  [RIGHT.y   UP.y    FORWARD.y    POS.y ]
	  [RIGHT.z   UP.y    FORWARD.z    POS.z ]
	  [   0        0         0          S   ]

	  [00   10    20    30 ]
	  [01   11    21    31 ]
	  [02   12    22    32 ]
	  [03   13    23    33 ]
*/

	template <typename T>
	struct Matrix<4, 4, T>
	{
		union
		{
			T Points[4][4];
			struct { Vector<4, T> Right; Vector<4, T> Up; Vector<4, T> Forward; Vector<4, T> Col3; };
		};
	};

	template<typename T>
	using Matrix4x4 = Matrix<4, 4, T>;

	extern const Matrix4x4<float> Matrix4x4f_Identity;
}