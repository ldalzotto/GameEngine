#include "StringAlgorithm.hpp"

#include <string.h>

namespace _Core
{
	StringSlice String_buildSlice(String* p_string)
	{
		return StringSlice{ p_string->Memory, 0, strlen(p_string->Memory) };
	};

	void String_freeIteratorOfStrings(VectorIteratorT<String>* l_strings)
	{
		while (VectorIteratorT_moveNext(l_strings))
		{
			String_free(l_strings->Current);
		}
	};
	
	void String_split(StringSlice* p_stringSlice, StringSlice* p_splitString, VectorT<String>* out_splits)
	{
		size_t l_index = p_stringSlice->Begin;
		size_t l_old_index = l_index;
		char* l_cursor = p_stringSlice->Memory + p_stringSlice->Begin;

		// StringSlice l_splitStringSlice = { p_splitString , 0, strlen(p_splitString) };
		StringSlice l_localStringSlice = *p_stringSlice;

		while (true)
		{
			l_localStringSlice.Begin = l_index;

			if (String_find_v2(&l_localStringSlice, p_splitString, &l_index))
			{
				size_t l_splittedStringSize = l_index - l_old_index;
				String l_str; String_alloc(&l_str, l_splittedStringSize);
				StringSlice l_slice; l_slice.Memory = l_cursor; l_slice.Begin = 0; l_slice.End = l_splittedStringSize;
				String_append(&l_str, &l_slice);
				// GenericArray_isertArrayAt_realloc((GenericArray*)&l_str, &l_insertedSplit, 0);

				VectorT_pushBack(out_splits, &l_str);

				l_index += 1;
				l_old_index = l_index;
				l_cursor += l_splittedStringSize;
				l_cursor += 1;
			}
			else if (l_index <= p_stringSlice->End)
			{
				l_index = p_stringSlice->End - 1;

				size_t l_splittedStringSize = l_index - l_old_index;
				String l_str; String_alloc(&l_str, l_splittedStringSize);
				StringSlice l_slice; l_slice.Memory = l_cursor; l_slice.Begin = 0; l_slice.End = l_splittedStringSize;
				String_append(&l_str, &l_slice);
				// GenericArray_isertArrayAt_realloc((GenericArray*)&l_str, &l_insertedSplit, 0);

				VectorT_pushBack(out_splits, &l_str);

				l_index += 1;
				l_old_index = l_index;
				l_cursor += l_splittedStringSize;
				l_cursor += 1;

				return;
			}
			else
			{
				return;
			}
		}
	};

	bool String_find_v2(StringSlice* p_stringSlice, StringSlice* p_comparedStr, size_t* p_outfoundIndex)
	{
		size_t l_stringSlice_size = p_stringSlice->End - p_stringSlice->Begin;
		if (l_stringSlice_size > 0)
		{
			for (*p_outfoundIndex = p_stringSlice->Begin; *p_outfoundIndex < p_stringSlice->End; (*p_outfoundIndex)++)
			{
				size_t l_endIndex = (*p_outfoundIndex) + (p_comparedStr->End - p_comparedStr->Begin);
				if (l_endIndex > p_stringSlice->End)
				{
					break;
				}

				char* l_pstringCompareBegin = p_stringSlice->Memory + *p_outfoundIndex;
				char* l_compareStringBegin = p_comparedStr->Memory + p_comparedStr->Begin;
				if (*l_pstringCompareBegin == *l_compareStringBegin)
				{
					if (memcmp(l_pstringCompareBegin, l_compareStringBegin, sizeof(char) * (p_comparedStr->End - p_comparedStr->Begin)) == 0)
					{
						return true;
					}
				}
			}
		}

		return false;
	};

	bool String_equals(StringSlice* p_sourceSlice, StringSlice* p_comparedSlice)
	{
		size_t l_sourceSlice_size = (p_sourceSlice->End - p_sourceSlice->Begin);
		size_t l_comparedSlice_size = (p_comparedSlice->End - p_comparedSlice->Begin);

		if (l_sourceSlice_size > 0 && l_comparedSlice_size > 0 && l_sourceSlice_size == l_comparedSlice_size)
		{
			return memcmp(p_sourceSlice->Memory + p_sourceSlice->Begin, p_comparedSlice->Memory + p_comparedSlice->Begin, l_comparedSlice_size * sizeof(char)) == 0;
		}

		return false;
	};
}