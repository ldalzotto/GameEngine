#pragma once

#include "ARRAY.h"
#include "STRING_def.h"

inline void Arr_Alloc_String(ARRAY_STRING_PTR p_array, size_t p_initialCapacity) { Arr_Alloc((ARRAY_PTR)p_array, sizeof(STRING), p_initialCapacity); };
inline void Arr_Free_String(ARRAY_STRING_PTR p_array) { Arr_Free((ARRAY_PTR)p_array); };
inline void Arr_PushBackRealloc_String(ARRAY_STRING_PTR p_array, STRING_PTR p_element) { Arr_PushBackRealloc((ARRAY_PTR)p_array, sizeof(STRING), (char*)p_element); };
inline void Arr_Clear_String(ARRAY_STRING_PTR p_array) { Arr_Clear((ARRAY_PTR)p_array); };

void String_Alloc(STRING_PTR p_string, size_t p_initialCapacity);
void String_Free(STRING_PTR p_string);
void String_AppendRaw(STRING_PTR p_string, char* p_appended);
void String_AppendSlice(STRING_PTR p_string, STRINGSLICE_PTR p_appended);
void String_Clear(STRING_PTR p_string);

bool String_Find(STRINGSLICE_PTR p_stringSlice, STRINGSLICE_PTR p_comparedStr, size_t* p_outfoundIndex);
void String_Split(STRINGSLICE_PTR p_stringSlice, STRINGSLICE_PTR p_splitString, ARRAY_STRING_PTR out_splits);
bool String_Equals(STRINGSLICE_PTR p_sourceSlice, STRINGSLICE_PTR p_comparedSlice);