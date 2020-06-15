#pragma once

#include "glm/glm.hpp"

namespace _GameEngine::_Math
{
	struct Vector3f;
	struct Vector4f;
}

namespace _GameEngine::_Math
{
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
	
		void _c0(Vector4f* p_out);
		void _c1(Vector4f* p_out);
		void _c2(Vector4f* p_out);
		void _c3(Vector4f* p_out);

		void _set_c0(Vector4f* p_col);
		void _set_c1(Vector4f* p_col);
		void _set_c2(Vector4f* p_col);
		void _set_c3(Vector4f* p_col);

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