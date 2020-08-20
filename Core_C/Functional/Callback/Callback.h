#pragma once

#include <stdbool.h>
#include "DataStructures/ARRAY.h"
#include "Error/ErrorHandler.h"

typedef struct Callback_TYP
{
	void (*Function)(void* p_closure, void* p_input);
	void* Closure;
} Callback, *Callback_PTR;

typedef struct ARRAY_CALLBACK_TYP
{
	ARRAY_TYPE_DEFINITION(Callback)
}ARRAY_CALLBACK, * ARRAY_CALLBACK_PTR;

inline void Arr_Alloc_Callback(ARRAY_CALLBACK_PTR p_array, size_t p_initialCapacity) { Arr_Alloc((ARRAY_PTR)p_array, sizeof(Callback), p_initialCapacity); };
inline void Arr_Free_Callback(ARRAY_CALLBACK_PTR p_array) { Arr_Free((ARRAY_PTR)p_array); };
inline void Arr_PushBackRealloc_Callback(ARRAY_CALLBACK_PTR p_array, Callback_PTR p_value) { HANDLE_ERR(Arr_PushBackRealloc((ARRAY_PTR)p_array, sizeof(Callback), (char*)p_value)); };
inline void Arr_Erase_Callback(ARRAY_CALLBACK_PTR p_array, size_t p_index) { HANDLE_ERR(Arr_Erase((ARRAY_PTR)p_array, sizeof(Callback), p_index)); };

bool Callback_equals(Callback_PTR p_left, Callback_PTR p_right);

void Callback_call(Callback_PTR p_callback, void* p_input);
