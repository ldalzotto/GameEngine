#pragma once

namespace _MathV2
{

	void RVector_min_specification(const float* p_left, const float* p_right, float* p_out, short p_size);
	void RVector_min_specification(const double* p_left, const double* p_right, double* p_out, short p_size);

	void RVector_normalize_specialization(const float* p_vec, short int p_size, float* p_out);


}