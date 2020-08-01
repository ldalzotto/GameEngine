#pragma once

#include <stdint.h>

namespace _Core
{
	bool Equals_ptr(void** p_left, void** p_right);
	bool Equals_double(const double* p_left, const  double* p_right);
	bool Equals_float(const float* p_left, const float* p_right);
	bool Equals_int16t(int16_t* p_left, int16_t* p_right);
	bool Equals_sizet(size_t* p_left, size_t* p_right);

}