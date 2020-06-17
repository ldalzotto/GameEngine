#include "Matrix.h"

#include <string>
#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{

	float Matrix4x4f::get_unsafe(int p_col, int p_line)
	{
		return *(float*)(((char*)&_00) + (sizeof(float) * ((4 * p_col) + p_line)));
	};

	void Matrix4x4f::_c0(Vector4f* p_out) { p_out->x = _00; p_out->y = _01; p_out->z = _02; p_out->w = _03;	};
	void Matrix4x4f::_c1(Vector4f* p_out) { p_out->x = _10; p_out->y = _11; p_out->z = _12; p_out->w = _13;	};
	void Matrix4x4f::_c2(Vector4f* p_out) { p_out->x = _20; p_out->y = _21; p_out->z = _22; p_out->w = _23;	};
	void Matrix4x4f::_c3(Vector4f* p_out) { p_out->x = _30; p_out->y = _31; p_out->z = _32; p_out->w = _33;	};

	void Matrix4x4f::_set_c0(Vector4f* p_col) { _00 = p_col->x; _01 = p_col->y; _02 = p_col->z; _03 = p_col->w; };
	void Matrix4x4f::_set_c1(Vector4f* p_col) { _10 = p_col->x; _11 = p_col->y; _12 = p_col->z; _13 = p_col->w; };
	void Matrix4x4f::_set_c2(Vector4f* p_col) { _20 = p_col->x; _21 = p_col->y; _22 = p_col->z; _23 = p_col->w; };
	void Matrix4x4f::_set_c3(Vector4f* p_col) { _30 = p_col->x; _31 = p_col->y; _32 = p_col->z; _33 = p_col->w; };

	void Matrix4x4f::_set_c0(Vector3f* p_col) { _00 = p_col->x; _01 = p_col->y; _02 = p_col->z; };
	void Matrix4x4f::_set_c1(Vector3f* p_col) { _10 = p_col->x; _11 = p_col->y; _12 = p_col->z; };
	void Matrix4x4f::_set_c2(Vector3f* p_col) { _20 = p_col->x; _21 = p_col->y; _22 = p_col->z; };
	void Matrix4x4f::_set_c3(Vector3f* p_col) { _30 = p_col->x; _31 = p_col->y; _32 = p_col->z; };

	void Matrix3x3f::_c0(Vector3f* p_out) { p_out->x = _00; p_out->y = _01; p_out->z = _02; };
	void Matrix3x3f::_c1(Vector3f* p_out) { p_out->x = _10; p_out->y = _11; p_out->z = _12; };
	void Matrix3x3f::_c2(Vector3f* p_out) { p_out->x = _20; p_out->y = _21; p_out->z = _22; };

	void Matrix4x4f::right(Vector4f* p_out) { p_out->x = _00; p_out->y = _10; p_out->z = _20; p_out->w = _30; };
	void Matrix4x4f::up(Vector4f* p_out) { p_out->x = _01; p_out->y = _11; p_out->z = _21; p_out->w = _31; };
	void Matrix4x4f::forward(Vector4f* p_out) { p_out->x = _02; p_out->y = _12; p_out->z = _22; p_out->w = _32; };

	Matrix4x4f Matrix4x4f_identity()
	{
		Matrix4x4f l_identity{};
		l_identity._00 = 1.0f;
		l_identity._11 = 1.0f;
		l_identity._22 = 1.0f;
		l_identity._33 = 1.0f;
		return l_identity;
	};

	void Matrix4x4f_copy(Matrix4x4f* p_source, Matrix4x4f* p_target)
	{
		memcpy(p_target, p_source, sizeof(Matrix4x4f));
	};

}