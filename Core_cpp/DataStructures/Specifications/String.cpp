#pragma once

#include "String.hpp"
#include <string.h>

#include "VectorT.hpp"
#include "ArrayT.hpp"



namespace _Core
{

	void String_alloc(String* p_string, size_t p_initialCapacity)
	{
		VectorT_alloc(p_string, p_initialCapacity);

		if (p_string->Size > 0)
		{
			p_string->Size = 0;
		}
		char l_nulChar = (char)NULL;
		
		VectorT_pushBack(p_string, &l_nulChar);
	};

	void String_free(String* p_string)
	{
		VectorT_free((VectorT<char>*)p_string);
	};

	void String_append(String* p_string, char* p_appended)
	{
		ArrayT<char> l_appededArray = ArrayT_fromCStyleArray(p_appended, strlen(p_appended));

		if (p_string->Size >= 2)
		{
			GenericArray_isertArrayAt_realloc((GenericArray*)p_string, (GenericArray*)&l_appededArray, CORE_STRING_CHAR_NB(p_string));
		}
		else
		{
			GenericArray_isertArrayAt_realloc((GenericArray*)p_string, (GenericArray*)&l_appededArray, 0);
		}
	};

	void String_append(String* p_string, StringSlice* p_appended)
	{
		char* l_begin = p_appended->Memory + p_appended->Begin;
		GenericArray l_appededArray; l_appededArray.Memory = l_begin;
		l_appededArray.Capacity = (p_appended->End - p_appended->Begin);
		l_appededArray.Size = (p_appended->End - p_appended->Begin);
		l_appededArray.ElementSize = sizeof(char);

		if (p_string->Size >= 2)
		{
			GenericArray_isertArrayAt_realloc((GenericArray*)p_string, &l_appededArray, CORE_STRING_CHAR_NB(p_string));
		}
		else
		{
			GenericArray_isertArrayAt_realloc((GenericArray*)p_string, &l_appededArray, 0);
		}
	};
		
	

	void String_clear(String* p_string)
	{
		p_string->Size = 0;
		char l_nulChar = (char)NULL;
		VectorT_pushBack(p_string, &l_nulChar);
	};

}