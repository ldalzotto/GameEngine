#pragma once

#include "VectorStructuresC_def.h"
#include "DataStructures/ARRAY.h"


inline void Arr_Alloc_Vector3F(Array_Vector3f_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc(&p_arr->array, sizeof(Vector3f), p_initialCapacity); };
inline void Arr_Free_Vector3F(Array_Vector3f_PTR p_arr) { Arr_Free(&p_arr->array); };
inline char Arr_PushBackRealloc_Vector3F(Array_Vector3f_PTR p_arr, Vector3f_PTR p_value) { return Arr_PushBackRealloc(&p_arr->array, sizeof(Vector3f), (char*)p_value); };
inline char Arr_PushBackNoRealloc_Vector3F(Array_Vector3f_PTR p_arr, Vector3f_PTR p_value) { return Arr_PushBackNoRealloc(&p_arr->array, sizeof(Vector3f), (char*)p_value); };
inline void Arr_BuildIterator_Vector3F(Array_Vector3f_PTR p_arr, Array_Vector3f_Iterator_PTR p_iter) { Arr_BuildIterator(&p_arr->array, sizeof(Vector3f), (ArrayIterator_PTR)p_iter); }
inline bool Iter_MoveNext_Vector3F(Array_Vector3f_Iterator_PTR p_iter) { return Iter_MoveNext((ArrayIterator_PTR)p_iter, sizeof(Vector3f)); }

inline void Arr_Alloc_Vector3C(ARRAY_Vector3c_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc(&p_arr->array, sizeof(Vector3c), p_initialCapacity); };
inline void Arr_Free_Vector3C(ARRAY_Vector3c_PTR p_arr) { Arr_Free(&p_arr->array); };
