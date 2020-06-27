#pragma once

#include <string>

namespace _GameEngine::_Core
{
	bool Vector_equalsPointerComparator(void** p_element, void** p_userObject);
	bool Vector_equalsStringComparator(std::string* p_element, std::string* p_userObject);

	short SizeTSortCompararator(size_t p_left, size_t p_right);
	short Uint16tSortComparator(uint16_t p_left, uint16_t p_right);
	short FloatSortComparator(float p_left, float p_right);
}