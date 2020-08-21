#include "DataStructures/String.h"
#include "DataStructures/ARRAY.h"

#include <string.h>

void String_Alloc(String_PTR p_string, size_t p_initialCapacity)
{
	Arr_Alloc((Array_PTR)p_string, sizeof(char), p_initialCapacity);

	if (p_string->Size > 0)
	{
		p_string->Size = 0;
	}
	char l_nulChar = (char)NULL;

	Arr_PushBackRealloc((Array_PTR)p_string, sizeof(char), &l_nulChar);
};

void String_Free(String_PTR p_string)
{
	Arr_Free((Array_PTR)p_string);
};

void String_AppendRaw(String_PTR p_string, char* p_appended)
{
	size_t l_insertedStringLength = strlen(p_appended);
	if (p_string->Size >= 2)
	{
		Arr_InsertAtRealloc((Array_PTR)p_string, sizeof(char), p_appended, l_insertedStringLength, p_string->Size - 1);
	}
	else
	{
		Arr_InsertAtRealloc((Array_PTR)p_string, sizeof(char), p_appended, l_insertedStringLength, 0);
	}
};

void String_AppendSlice(String_PTR p_string, StringSLICE_PTR p_appended)
{
	char* l_appendedString = p_appended->Memory + p_appended->Begin;
	size_t l_charactersInserted = (p_appended->End - p_appended->Begin);

	if (p_string->Size >= 2)
	{
		Arr_InsertAtRealloc((Array_PTR)p_string, sizeof(char), l_appendedString, l_charactersInserted, p_string->Size - 1);
	}
	else
	{
		Arr_InsertAtRealloc((Array_PTR)p_string, sizeof(char), l_appendedString, l_charactersInserted, 0);
	}
};

void String_Clear(String_PTR p_string)
{
	p_string->Size = 0;
	char l_nulChar = (char)NULL;
	Arr_PushBackRealloc((Array_PTR)p_string, sizeof(char), &l_nulChar);
};

bool String_Find(StringSLICE_PTR p_stringSlice, StringSLICE_PTR p_comparedStr, size_t* p_outfoundIndex)
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

void String_Split(StringSLICE_PTR p_stringSlice, StringSLICE_PTR p_splitString, Array_String_PTR out_splits)
{
	size_t l_index = p_stringSlice->Begin;
	size_t l_old_index = l_index;
	char* l_cursor = p_stringSlice->Memory + p_stringSlice->Begin;

	// StringSlice l_splitStringSlice = { p_splitString , 0, strlen(p_splitString) };
	StringSLICE l_localStringSlice = *p_stringSlice;

	while (true)
	{
		l_localStringSlice.Begin = l_index;

		if (String_Find(&l_localStringSlice, p_splitString, &l_index))
		{
			size_t l_splittedStringSize = l_index - l_old_index;
			String l_str; String_Alloc(&l_str, l_splittedStringSize);
			StringSLICE l_slice; l_slice.Memory = l_cursor; l_slice.Begin = 0; l_slice.End = l_splittedStringSize;
			String_AppendSlice(&l_str, &l_slice);

			Arr_PushBackRealloc_String(out_splits, &l_str);

			l_index += 1;
			l_old_index = l_index;
			l_cursor += l_splittedStringSize;
			l_cursor += 1;
		}
		else if (l_index <= p_stringSlice->End)
		{
			l_index = p_stringSlice->End - 1;

			size_t l_splittedStringSize = l_index - l_old_index;
			String l_str; String_Alloc(&l_str, l_splittedStringSize);
			StringSLICE l_slice; l_slice.Memory = l_cursor; l_slice.Begin = 0; l_slice.End = l_splittedStringSize;
			String_AppendSlice(&l_str, &l_slice);

			Arr_PushBackRealloc_String(out_splits, &l_str);

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

bool String_Equals(StringSLICE_PTR p_sourceSlice, StringSLICE_PTR p_comparedSlice)
{
	size_t l_sourceSlice_size = (p_sourceSlice->End - p_sourceSlice->Begin);
	size_t l_comparedSlice_size = (p_comparedSlice->End - p_comparedSlice->Begin);

	if (l_sourceSlice_size > 0 && l_comparedSlice_size > 0 && l_sourceSlice_size == l_comparedSlice_size)
	{
		return memcmp(p_sourceSlice->Memory + p_sourceSlice->Begin, p_comparedSlice->Memory + p_comparedSlice->Begin, l_comparedSlice_size * sizeof(char)) == 0;
	}

	return false;
};