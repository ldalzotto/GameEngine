#include "StringAlgorithm.h"

#include <stdexcept>
#include <string.h> 
#include "String.h"

namespace _GameEngine::_Core
{
	void String_append(_Core::String* p_string, char* p_appended, size_t p_charNb)
	{
		if (p_string->Vector.size() >= 2)
		{
			p_string->Vector.insertAt(p_appended, p_charNb, p_string->Vector.size() - 1);
		}
		else
		{
			p_string->Vector.insertAt(p_appended, p_charNb, 0);
		}
	};

	void String_append(_Core::String* p_string, char* p_appended)
	{
		String_append(p_string, p_appended, strlen(p_appended));
	};

	bool String_find(_Core::String* p_string, char* p_comparedStr, size_t p_comparedCharNb, size_t p_startIndex, size_t* p_outfoundIndex)
	{
		if (p_string->Vector.size() > 0)
		{
			for (*p_outfoundIndex = p_startIndex; *p_outfoundIndex < p_string->Vector.size(); (*p_outfoundIndex)++)
			{
				size_t l_endIndex = (*p_outfoundIndex) + p_comparedCharNb;
				if (l_endIndex > String_charNb(p_string))
				{
					break;
				}

				char* l_pstringCompareBegin = p_string->Vector.at(*p_outfoundIndex);
				char* l_compareStringBegin = p_comparedStr;
				if (*l_pstringCompareBegin == *l_compareStringBegin)
				{
					if (memcmp(l_pstringCompareBegin, l_compareStringBegin, sizeof(char) * p_comparedCharNb) == 0)
					{
						return true;
					}
				}
			}
		}

		return false;
	};

	void String_substring(_Core::String* p_string, size_t p_beginIncluded, size_t p_endIncluded, _Core::String* p_newString)
	{
		if (p_beginIncluded < 0 || p_endIncluded + 1 > String_charNb(p_string))
		{
			throw std::out_of_range("String_substring out of range.");
		}
		else if (p_beginIncluded > p_endIncluded)
		{
			throw std::runtime_error("String_substring invalid indices.");
		}

		size_t l_substringSize = p_endIncluded + 1 - p_beginIncluded;
		String_append(p_newString, p_string->Vector.at(p_beginIncluded), l_substringSize);
	};

	bool String_equals(_Core::String* p_string, char* p_comparedStr)
	{
		return memcmp(p_string->c_str(), p_comparedStr, p_string->Vector.size() * sizeof(char)) == 0;
	};
}