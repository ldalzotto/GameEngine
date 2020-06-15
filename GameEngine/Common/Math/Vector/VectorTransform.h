#pragma once

namespace _GameEngine::_Math
{
	struct Vector3f;
}

namespace _GameEngine::_Math
{
	void Vector3f_mul(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_mul(Vector3f* p_vector, float p_value);
}