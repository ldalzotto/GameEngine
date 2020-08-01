#pragma once

namespace _GameEngine::_Math
{
	struct Vector2f;
	struct Vector2d;
	struct Vector3f;
	struct Vector4f;
}

namespace _GameEngine::_Math
{
	float Vector3f_length(Vector3f* p_vec);
	
	void Vector3f_mul(Vector3f* p_vector, float p_value, Vector3f* p_out);
	void Vector3f_add(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	
	float Vector3f_dot(Vector3f* p_vector, Vector3f* p_other);
	void Vector3f_cross(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_project(Vector3f* p_vector, Vector3f* p_projectedOn, Vector3f* p_out);
	float Vector3f_angle_signed(Vector3f* p_p1, Vector3f* p_p2);
	
	float Vector3f_angle(Vector3f* p_vector, Vector3f* p_other);

	bool Vector4f_equals(Vector4f* left, Vector4f* right);
	void Vector4f_build(Vector3f* p_xyz, float p_w, Vector4f* out);
	void Vector4f_build(Vector2f* p_xy, float p_z, float p_w, Vector4f* out);

	void Vector4f_mul(Vector4f* p_vect, float p_value, Vector4f* p_out);
	void Vector4f_homogenize_w(Vector4f* p_vect, Vector4f* p_out);

	float Vector4f_length(Vector4f* p_vec);
	void Vector4f_add(Vector4f* p_vector, Vector4f* p_other, Vector4f* p_out);
	void Vector4f_add(Vector4f* p_vector, Vector3f* p_other, Vector4f* p_out);

}