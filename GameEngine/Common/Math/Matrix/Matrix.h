#pragma once

namespace _GameEngine::_Math
{
	struct Vector3f;
	struct Vector4f;
}

namespace _GameEngine::_Math
{

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
	struct Matrix4x4f
	{
		float _00;
		float _01;
		float _02;
		float _03;
		float _10;
		float _11;
		float _12;
		float _13;
		float _20;
		float _21;
		float _22;
		float _23;
		float _30;
		float _31;
		float _32;
		float _33;
	};

	Matrix4x4f Matrix4x4f_identity();
	void Matrix4x4f_copy(Matrix4x4f* p_source, Matrix4x4f* p_target);

	struct Matrix3x3f
	{
		float _00;
		float _01;
		float _02;
		float _10;
		float _11;
		float _12;
		float _20;
		float _21;
		float _22;
	};

}