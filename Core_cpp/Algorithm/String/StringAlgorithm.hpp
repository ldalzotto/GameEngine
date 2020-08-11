#pragma once

#include "DataStructures/Specifications/String.hpp"

namespace _Core
{
	void String_split(StringSlice* p_stringSlice, StringSlice* p_splitString, VectorT<String>* out_splits);
	bool String_find_v2(StringSlice* p_stringSlice, StringSlice* p_comparedStr, size_t* p_outfoundIndex);
	bool String_equals(StringSlice* p_sourceSlice, StringSlice* p_comparedSlice);

	void String_freeIteratorOfStrings(VectorIteratorT<String>* l_strings);

	StringSlice String_buildSlice(String* p_string);
}