#pragma once

#include "VectorT.hpp"

namespace _Core
{
	struct String : VectorT<char> { };

	void String_alloc(String* p_string, size_t p_initialCapacity);
	void String_append(String* p_string, char* p_appended);
	bool String_find(String* p_string, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex);
}