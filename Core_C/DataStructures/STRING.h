#pragma once

#include "ARRAY.h"
#include "String_def.h"

inline void Arr_Alloc_String(Array_String_PTR p_array, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(String), p_initialCapacity); };
inline void Arr_Free_String(Array_String_PTR p_array) { Arr_Free((Array_PTR)p_array); };
inline void Arr_PushBackRealloc_String(Array_String_PTR p_array, String_PTR p_element) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(String), (char*)p_element); };
// inline void Arr_PushBackNoRealloc_String(Array_String_PTR p_array, String_PTR p_element) { Arr_PushBackRealloc((Array_PTR)p_array, sizeof(String), (char*)p_element); };
inline void Arr_Clear_String(Array_String_PTR p_array) { Arr_Clear((Array_PTR)p_array); };

void String_Alloc(String_PTR p_string, size_t p_initialCapacity);
void String_Free(String_PTR p_string);
void String_AppendRawRealloc(String_PTR p_string, char* p_appended);
void String_AppendRawNoRealloc(String_PTR p_string, char* p_appended);
void String_AppendSliceRealloc(String_PTR p_string, StringSLICE_PTR p_appended);
void String_ClearRealloc(String_PTR p_string);
void String_ClearNoRealloc(String_PTR p_string);

bool String_Find(StringSLICE_PTR p_stringSlice, StringSLICE_PTR p_comparedStr, size_t* p_outfoundIndex);
void String_Split(StringSLICE_PTR p_stringSlice, StringSLICE_PTR p_splitString, Array_String_PTR out_splits);
bool String_Equals(StringSLICE_PTR p_sourceSlice, StringSLICE_PTR p_comparedSlice);