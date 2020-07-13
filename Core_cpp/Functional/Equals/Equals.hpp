#pragma once

#include <stdint.h>

namespace _Core
{
	typedef bool(*Equals_function)(void* p_left, void* p_right);

	bool Core_Equals(float* p_left, float* p_right);
	bool Core_Equals(int16_t* p_left, int16_t* p_right);
	bool Core_Equals(size_t* p_left, size_t* p_right);

}