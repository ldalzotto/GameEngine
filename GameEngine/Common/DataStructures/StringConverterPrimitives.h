#pragma once

namespace _GameEngine::_Core
{
	struct String;
}

namespace _GameEngine::_Core
{
	void String_appendPointer(String* p_string, void* p_ptr);
	void String_appendDouble(String* p_string, double p_value);
	void String_appendFloat(String* p_string, float p_value);
}