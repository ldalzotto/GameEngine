#pragma once

namespace _MathV2
{
	extern const float RIGHT_arr[3];
	extern const float UP_arr[3];
	extern const float FORWARD_arr[3];

	bool RVector_2_equals(const float p_left[2], const float p_right[2]);
	bool RVector_2_equals(const double p_left[2], const double p_right[2]);
	bool RVector_3_equals(const float p_left[3], const float p_right[3]);

	void RVector_3_add(const float p_left[3], const float p_right[3], float p_out[3]);

	void RVector_min_specification(const float* p_left, const float* p_right, float* p_out, short p_size);
	void RVector_min_specification(const double* p_left, const double* p_right, double* p_out, short p_size);

	void RVector_3_mul(const float p_left[3], const float p_right, float p_out[3]);
	void RVector_3_mul(const float p_left[3], const float p_right[3], float p_out[3]);
	void RVector_4_mul(const float p_left[4], const float p_right, float p_out[4]);

	void RVector_3_inv(const float p_vec[3], float p_out[3]);

	float RVector_length_specialization(const float* p_vec, short int p_size);
	float RVector_length_specialization(const double* p_vec, short int p_size);
	
	float RVector_3_distance(const float p_start[3], const float p_end[3]);

	void RVector_normalize_specialization(const float* p_vec, short int p_size, float* p_out);

	float RVector_3_dot(const float p_left[3], const float p_right[3]);
	
	void RVector_3_cross(const float p_left[3], const float p_right[3], float p_out[3]);

	void RVector_3_project(const float p_vec[3], const float p_projectedOn[3], float p_out[3]);

	float RVector_3_angle(const float p_begin[3], const  float p_end[3]);
	float RVector_3_angle_normalized(const float p_begin[3], const float p_end[3]);

	short int RVector_3_angleSign(const float p_begin[3], const  float p_end[3], const float p_referenceAxis[3]);

	void RVector_3_rotate(const float p_vector[3], const float p_rotation[4], float p_out[3]);

}