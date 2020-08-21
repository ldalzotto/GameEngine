#pragma once

#include <stdbool.h>
#include "DataStructures/ARRAY.h"
#include "Error/ErrorHandler.h"

typedef struct Callback_TYP
{
	void (*Function)(void* p_closure, void* p_input);
	void* Closure;
} Callback, *Callback_PTR;

typedef struct Array_Callback_TYP
{
	ARRAY_TYPE_DEFINITION(Callback)
}Array_Callback, * Array_Callback_PTR;

inline void Arr_Alloc_Callback(Array_Callback_PTR p_array, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_array, sizeof(Callback), p_initialCapacity); };
inline void Arr_Free_Callback(Array_Callback_PTR p_array) { Arr_Free((Array_PTR)p_array); };
inline void Arr_PushBackRealloc_Callback(Array_Callback_PTR p_array, Callback_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((Array_PTR)p_array, sizeof(Callback), (char*)p_value)); };
inline void Arr_Erase_Callback(Array_Callback_PTR p_array, size_t p_index) { HANDLE_ERR(Arr_Erase((Array_PTR)p_array, sizeof(Callback), p_index)); };

bool Callback_equals(Callback_PTR p_left, Callback_PTR p_right);

void Callback_call(Callback_PTR p_callback, void* p_input);
