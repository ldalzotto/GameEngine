#include "ElementComparators.h"

namespace _GameEngine::_Core
{
	bool Vector_equalsPointerComparator(void** p_element, void** p_userObject)
	{
		return *p_element == *p_userObject;
	};

	bool Vector_equalsStringComparator(std::string* p_element, std::string* p_userObject)
	{
		return (*p_element).compare(p_userObject->data()) == 0;
	};


	short SizeTSortCompararator(size_t p_left, size_t p_right)
	{
		if (p_left > p_right) { return 1; }
		else if (p_left < p_right) { return -1; }
		else { return 0; }
	};

	short Uint16tSortComparator(uint16_t p_left, uint16_t p_right)
	{
		if (p_left > p_right) { return 1; }
		else if (p_left < p_right) { return -1; }
		else { return 0; }
	}

	short FloatSortComparator(float p_left, float p_right)
	{
		if (p_left > p_right) { return 1; }
		else if (p_left < p_right) { return -1; }
		else { return 0; }
	};
}