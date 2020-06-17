#include "Matrix.h"

#include <string>

namespace _GameEngine::_Math
{


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