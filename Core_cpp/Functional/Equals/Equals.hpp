#pragma once

#include <stdint.h>

namespace _Core
{
	bool Equals_ptr(void** p_left, void** p_right);
	bool Equals_double(double* p_left, double* p_right);
	bool Equals_float(float* p_left, float* p_right);
	bool Equals_int16t(int16_t* p_left, int16_t* p_right);
	bool Equals_sizet(size_t* p_left, size_t* p_right);

}