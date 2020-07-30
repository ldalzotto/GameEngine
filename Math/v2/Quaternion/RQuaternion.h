#pragma once

namespace _MathV2
{	
	void RQuaternion_build(const float p_vec[3], const float p_scal, float p_out[4]);
	void RQuaternion_fromDirection(const float p_vec[3], float p_out[4]);
	void RQuaternion_fromAxis(const float p_axis[3][3], float p_out[4]);
	void RQuaternion_fromEulerAngle(const float p_eulerAngle[3], float p_out[4]);
	void RQuaternion_fromTo(const float p_from[3], const  float p_to[3], float p_out[4]);
	void RQuaternion_conjugate(const float p_quat[4], float p_out[4]);
	void RQuaternion_normalize(const float p_quat[4], float p_out[4]);
	void RQuaternion_mul(const float p_left[4], const  float p_right[4], float p_out[4]);
	void RQuaternion_cross(const float p_left[4], const  float p_right[4], float p_out[4]);
	void RQuaternion_rotateAround(const float p_axis[3], const  float p_angle, float out_quat[4]);

	void RQuaternion_extractAxis(const float quat[4], float out_axis[3][3]);
}