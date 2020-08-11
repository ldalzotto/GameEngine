#pragma once

#include "VectorT.hpp"

#define CORE_STRING_CHAR_NB(in_string) (in_string)->Size - 1

namespace _Core
{
	struct String : VectorT<char> { };
	struct StringSlice
	{
		char* Memory;
		size_t Begin;
		size_t End;
	};

	void String_alloc(String* p_string, size_t p_initialCapacity);
	void String_free(String* p_string);
	void String_append(String* p_string, char* p_appended);
	void String_append(String* p_string, StringSlice* p_appended);
	void String_clear(String* p_string);
}