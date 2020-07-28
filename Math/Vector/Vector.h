#pragma once

namespace _Math
{

	void RVector_3_add(float p_left[3], float p_right[3], float p_out[3]);

	void RVector_3_mul(float p_left[3], float p_right, float p_out[3]);
	void RVector_4_mul(float p_left[4], float p_right, float p_out[4]);

	float RVector_3_length(float p_vec[3]);
	float RVector_4_length(float p_vec[4]);

	void RVector_3_normalize(float p_vec[3], float p_out[3]);

	float RVector_3_dot(float p_left[3], float p_right[3]);
	
	void RVector_3_cross(float p_left[3], float p_right[3], float p_out[3]);

	float RVector_3_angle(float p_begin[3], float p_end[3]);

}