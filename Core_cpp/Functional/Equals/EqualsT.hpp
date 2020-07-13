#pragma once

#include "Equals.hpp"

namespace _Core
{
	template <typename LEFT_TYPE, typename RIGHT_TYPE>
	using EqualsT_function = bool(*)(void* p_left, void* p_right);
}