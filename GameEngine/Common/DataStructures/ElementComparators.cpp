#include "ElementComparators.h"

namespace _GameEngine::_Core
{
	bool Vector_equalsStringComparator(std::string* p_element, std::string* p_userObject)
	{
		return (*p_element).compare(p_userObject->data()) == 0;
	};

	short FloatSortComparator(float p_left, float p_right)
	{
		if (p_left > p_right) { return 1; }
		else if (p_left < p_right) { return -1; }
		else { return 0; }
	};
}