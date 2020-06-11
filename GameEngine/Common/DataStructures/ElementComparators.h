#pragma once

#include <string>

namespace _GameEngine::_Core
{
	bool Vector_equalsPointerComparator(void** p_element, void** p_userObject);
	bool Vector_equalsStringComparator(std::string* p_element, std::string* p_userObject);
}