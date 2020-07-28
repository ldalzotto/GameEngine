#pragma once

namespace _Math
{	
	void RQuaternion_build(float p_vec[3], float p_scal, float p_out[4]);

	void RQuaternion_conjugate(float p_quat[4], float p_out[4]);

	void RQuaternion_normalize(float p_quat[4], float p_out[4]);

	void RQuaternion_mul(float p_left[4], float p_right[4], float p_out[4]);

	void RQuaternion_rotateAround(float p_axis[3], float p_angle, float out_quat[4]);
}