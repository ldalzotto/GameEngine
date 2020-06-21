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
}