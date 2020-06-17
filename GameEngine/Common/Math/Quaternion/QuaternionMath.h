#pragma once

namespace _GameEngine::_Math
{
	struct Quaternionf;
	struct Matrix3x3f;
	struct Matrix4x4f;
	struct Vector3f;
}

namespace _GameEngine::_Math
{
	void Quaternionf_copy(Quaternionf* p_source, Quaternionf* p_target);
	bool Quaternionf_equals(Quaternionf* p_left, Quaternionf* p_right);

	void Quaternion_mul(Quaternionf* p_quaternion, Quaternionf* p_other, Quaternionf* out);
	void Quaternion_rotateAround(Quaternionf* p_quaternion, Vector3f* p_axis, float p_angle, Quaternionf* out);

	void Quaternion_fromEulerAngles(Vector3f* p_eulerAngles, Quaternionf* p_out);
	void Quaternion_fromEulerAngles(Vector3f& p_eulerAngles, Quaternionf* p_out);
	void Quaterion_toRotationMatrix(Quaternionf* p_quaternion, Matrix3x3f* out_matrix);
	void Quaterion_toRotationMatrix(Quaternionf* p_quaternion, Matrix4x4f* out_matrix);
}