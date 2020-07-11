#pragma once

#include <string>

namespace _GameEngine::_Core
{
	bool Vector_equalsStringComparator(std::string* p_element, std::string* p_userObject);

	short FloatSortComparator(float p_left, float p_right);
}