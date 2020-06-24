#include "StringConverterPrimitives.h"
#include <stdio.h>

namespace _GameEngine::_Core
{
	void String_appendPointer(String* p_string, void* p_ptr)
	{
		char l_allocatedMemory[19];
		snprintf(l_allocatedMemory, sizeof(char) * 19, "0x%p", p_ptr);
		p_string->append(l_allocatedMemory);
	};

	void String_appendDouble(String* p_string, double p_value)
	{
		char l_allocatedMemory[50];
		snprintf(l_allocatedMemory, sizeof(char) * 50, "%f", p_value);
		p_string->append(l_allocatedMemory);
	};
}