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
}