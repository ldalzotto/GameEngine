#pragma once

#include "glm/glm.hpp"

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

		float get_unsafe(int p_col, int p_line);
	
		void _c0(Vector4f* p_out);
		void _c1(Vector4f* p_out);
		void _c2(Vector4f* p_out);
		void _c3(Vector4f* p_out);

		void _set_c0(Vector4f* p_col);
		void _set_c1(Vector4f* p_col);
		void _set_c2(Vector4f* p_col);
		void _set_c3(Vector4f* p_col);

		void _set_c0(Vector3f* p_col);
		void _set_c1(Vector3f* p_col);
		void _set_c2(Vector3f* p_col);
		void _set_c3(Vector3f* p_col);

		void right(Vector4f* p_out);
		void up(Vector4f* p_out);
		void forward(Vector4f* p_out);

		glm::mat4x4 toGLM();
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

		void _c0(Vector3f* p_out);
		void _c1(Vector3f* p_out);
		void _c2(Vector3f* p_out);
	};

	Matrix4x4f Matrix4x4f_fromGLM(const glm::mat4x4& p_mat);

}