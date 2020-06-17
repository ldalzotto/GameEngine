#pragma once

namespace _GameEngine::_Math
{
	struct Vector3f;
}

namespace _GameEngine::_Math
{
	void Vector3f_mul(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_mul(Vector3f* p_vector, float p_value, Vector3f* p_out);
	void Vector3f_add(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_min(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	float Vector3f_dot(Vector3f* p_vector, Vector3f* p_other);
	void Vector3f_cross(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	float Vector3f_angle(Vector3f* p_vector, Vector3f* p_other);
}