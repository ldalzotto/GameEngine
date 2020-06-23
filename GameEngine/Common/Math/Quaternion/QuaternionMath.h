#pragma once

namespace _GameEngine::_Math
{
	struct Quaternionf;
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
	void Quaternion_toEulerAngles(Quaternionf* p_quaternion, Vector3f* p_eulerAngles);
	void Quaternion_extractAxis(Quaternionf* p_quaternion, Vector3f* out_right, Vector3f* out_up, Vector3f* out_forward);
	void Quaternion_fromAxis(Vector3f* p_right, Vector3f* p_up, Vector3f* p_forward, Quaternionf* out_quaternion);

}