#pragma once

#include "VectorStructuresC_def.h"
#include "DataStructures/ARRAY.h"

inline void Arr_Alloc_Vector3F(ARRAY_VECTOR3F_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc((ARRAY_PTR)p_arr, sizeof(VECTOR3F), p_initialCapacity); };
inline void Arr_Free_Vector3F(ARRAY_VECTOR3F_PTR p_arr) { Arr_Free((ARRAY_PTR)p_arr); };
inline char Arr_PushBackRealloc_Vector3F(ARRAY_VECTOR3F_PTR p_arr, VECTOR3F_PTR p_value) { return Arr_PushBackRealloc((ARRAY_PTR)p_arr, sizeof(VECTOR3F), (char*)p_value); };
inline void Arr_BuildIterator_Vector3F(ARRAY_VECTOR3F_PTR p_arr, ARRAY_VECTOR3F_ITERATOR_PTR p_iter) { Arr_BuildIterator((ARRAY_PTR)p_arr, sizeof(VECTOR3F), (ARRAY_ITERATOR_PTR)p_iter); }
inline bool Iter_MoveNext_Vector3F(ARRAY_VECTOR3F_ITERATOR_PTR p_iter) { return Iter_MoveNext((ARRAY_ITERATOR_PTR)p_iter, sizeof(VECTOR3F)); }