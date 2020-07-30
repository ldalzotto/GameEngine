#pragma once

namespace _MathV2
{
	extern float RIGHT_arr[3];
	extern float UP_arr[3];
	extern float FORWARD_arr[3];

	void RVector_3_add(float p_left[3], float p_right[3], float p_out[3]);
	void RVector_3_min(float p_left[3], float p_right[3], float p_out[3]);

	void RVector_3_mul(float p_left[3], float p_right, float p_out[3]);
	void RVector_4_mul(float p_left[4], float p_right, float p_out[4]);

	float RVector_3_length(float p_vec[3]);
	float RVector_4_length(float p_vec[4]);

	void RVector_3_normalize(float p_vec[3], float p_out[3]);

	float RVector_3_dot(float p_left[3], float p_right[3]);
	
	void RVector_3_cross(float p_left[3], float p_right[3], float p_out[3]);

	float RVector_3_angle(float p_begin[3], float p_end[3]);
	float RVector_3_angle_normalized(float p_begin[3], float p_end[3]);

	short int RVector_3_angleSign(float p_begin[3], float p_end[3], float p_referenceAxis[3]);

	void RVector_3_rotate(float p_vector[3], float p_rotation[4], float p_out[3]);
}