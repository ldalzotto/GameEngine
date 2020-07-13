#pragma once

#include "String.hpp"
#include <string.h>

#include "VectorT.hpp"
#include "ArrayT.hpp"

#define CORE_STRING_CHAR_NB(in_string) (in_string)->Size - 1

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
	
	bool String_find(String* p_string, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex)
	{
		ArrayT<char> l_comparedStr = ArrayT_fromCStyleArray(p_comparedStr, strlen(p_comparedStr));

		if (p_string->Size > 0)
		{
			for (*p_outfoundIndex = p_startIndex; *p_outfoundIndex < p_string->Size; (*p_outfoundIndex)++)
			{
				size_t l_endIndex = (*p_outfoundIndex) + l_comparedStr.Size;
				if (l_endIndex > CORE_STRING_CHAR_NB(p_string))
				{
					break;
				}

				char* l_pstringCompareBegin = (char*)GenericArray_at((GenericArray*)p_string, *p_outfoundIndex);
				char* l_compareStringBegin = p_comparedStr;
				if (*l_pstringCompareBegin == *l_compareStringBegin)
				{
					if (memcmp(l_pstringCompareBegin, l_compareStringBegin, sizeof(char) * l_comparedStr.Size) == 0)
					{
						return true;
					}
				}
			}
		}

		return false;
	};

}