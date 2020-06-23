#pragma once

namespace _GameEngine::_Math
{
	struct Vector3f;
	struct Vector4f;
}

namespace _GameEngine::_Math
{
	void Vector3f_build(Vector4f* p_xyz, Vector3f* out);

	bool Vector3f_equals(Vector3f* left, Vector3f* right);
	void Vector3f_normalize(Vector3f* p_vec);
	float Vector3f_length(Vector3f* p_vec);
	float Vector3f_distance(Vector3f* p_vec, Vector3f* p_target);

	void Vector3f_mul(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_mul(Vector3f* p_vector, float p_value, Vector3f* p_out);
	void Vector3f_add(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_min(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	
	float Vector3f_dot(Vector3f* p_vector, Vector3f* p_other);
	void Vector3f_cross(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	
	float Vector3f_angle(Vector3f* p_vector, Vector3f* p_other);

	bool Vector4f_equals(Vector4f* left, Vector4f* right);
	void Vector4f_build(Vector3f* p_xyz, float p_w, Vector4f* out);

	float Vector4f_length(Vector4f* p_vec);
	void Vector4f_add(Vector4f* p_vector, Vector4f* p_other, Vector4f* p_out);
	void Vector4f_add(Vector4f* p_vector, Vector3f* p_other, Vector4f* p_out);

}