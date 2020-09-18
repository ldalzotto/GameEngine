#pragma once

#include "VectorStructuresC_def.h"
#include "DataStructures/ARRAY.h"

ARRAY_ALLOC_FUNCTION(Vector2i, Array_Vector2i_PTR, Vector2i);
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(Vector2i, Array_Vector2i_PTR, Vector2i);
ARRAY_RESIZE_FUNCTION(Vector2i, Array_Vector2i_PTR, Vector2i);

inline void Arr_Alloc_Vector3F(Array_Vector3f_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc(&p_arr->array, sizeof(Vector3f), p_initialCapacity); };
inline void Arr_Free_Vector3F(Array_Vector3f_PTR p_arr) { Arr_Free(&p_arr->array); };
inline char Arr_PushBackRealloc_Vector3F(Array_Vector3f_PTR p_arr, Vector3f_PTR p_value) { return Arr_PushBackRealloc(&p_arr->array, sizeof(Vector3f), (char*)p_value); };
inline char Arr_PushBackNoRealloc_Vector3F(Array_Vector3f_PTR p_arr, Vector3f_PTR p_value) { return Arr_PushBackNoRealloc(&p_arr->array, sizeof(Vector3f), (char*)p_value); };

inline void Arr_Alloc_Vector3C(ARRAY_Vector3c_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc(&p_arr->array, sizeof(Vector3c), p_initialCapacity); };
inline void Arr_Free_Vector3C(ARRAY_Vector3c_PTR p_arr) { Arr_Free(&p_arr->array); };

ARRAY_ALLOC_FUNCTION(Vector2f, Array_Vector2f_PTR, Vector2f)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(Vector2f, Array_Vector2f_PTR, Vector2f)