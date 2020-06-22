#pragma once

#include "DataStructures/String.h"

namespace _GameEngine::_Core
{
	void String_appendPointer(String* p_string, void* p_ptr);
	void String_appendDouble(String* p_string, double p_value);
}