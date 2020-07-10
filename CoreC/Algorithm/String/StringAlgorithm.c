#include "StringAlgorithm.h"

#include <string.h>

#include "ErrorHandling/Errorhandling.h"
#include "DataStructures/Specifications/Array.h"
#include "Functional/Vector/VectorWriter.h"
#include "Functional/Vector/VectorAccessor.h"

#define CORE_STRING_CHAR_NB(in_string) (in_string)->Size - 1

void Core_string_append(struct Core_GenericArray* p_string, char* p_appended)
{
	Core_GenericArray l_appededArray = Core_array_fromCStyle(p_appended, sizeof(char), strlen(p_appended));

	if (p_string->Size >= 2)
	{
		ERR_THROW(p_string->Functions->Writer->InsertArrayAt(p_string, &l_appededArray, CORE_STRING_CHAR_NB(p_string)));
	}
	else
	{
		ERR_THROW(p_string->Functions->Writer->InsertArrayAt(p_string, &l_appededArray, 0));
	}
};

bool Core_string_find(struct Core_GenericArray* p_string, char* p_comparedStr, size_t p_startIndex, size_t* p_outfoundIndex)
{

	Core_GenericArray l_comparedStr = Core_array_fromCStyle(p_comparedStr, sizeof(char), strlen(p_comparedStr));

	if (p_string->Size > 0)
	{
		for (*p_outfoundIndex = p_startIndex; *p_outfoundIndex < p_string->Size; (*p_outfoundIndex)++)
		{
			size_t l_endIndex = (*p_outfoundIndex) + l_comparedStr.Size;
			if (l_endIndex > CORE_STRING_CHAR_NB(p_string))
			{
				break;
			}

			char* l_pstringCompareBegin = (char*)l_comparedStr.Functions->Accessor->At(p_string, *p_outfoundIndex);
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