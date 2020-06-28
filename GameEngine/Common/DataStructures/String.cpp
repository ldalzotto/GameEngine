#include "String.h"
#include <string.h>

namespace _GameEngine::_Core
{
	void String::alloc(size_t p_charNb)
	{
		this->Vector.alloc(p_charNb + 1);
		char l_nul = NULL;
		this->Vector.push_back(&l_nul);
	};

	void String::free()
	{
		this->Vector.free();
	};

	void String::reserve(size_t p_charNb) 
	{
		this->Vector.reserve(p_charNb);
	};

	char* String::c_str()
	{
		return (char*)this->Vector.Vector.Memory;
	};

	char* FixedString::c_str()
	{
		return (char*)this->Array.Array.Memory;
	};

	FixedString FixedString_interpret(char* p_source)
	{
		return FixedString_interpret(p_source, strlen(p_source) + 1);
	};

	FixedString FixedString_interpret(char* p_source, size_t p_sourceCharNb)
	{
		FixedString l_string{};
		ArrayT_interpret(&l_string.Array, p_source, p_sourceCharNb);
		return l_string;
	}

}