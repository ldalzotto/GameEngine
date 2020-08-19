#pragma once

#include <stdint.h>

namespace _Core
{

	typedef short int(*ElementSort_function)(void* p_sourceObject, void* p_comparedObject, void* p_userObject);

	struct ElementSorter
	{
		ElementSort_function Function;
		void* UserObject;

	};

}