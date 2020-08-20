#pragma once

#include "RenderedObject_def.h"
#include "DataStructures/ARRAY.h"

inline void Arr_Alloc_RenderedObjectHandle(ARRAY_REDEREDOBJECT_HANDLE_PTR p_array, size_t p_initialCapacity) { Arr_Alloc((ARRAY_PTR)p_array, sizeof(RENDEREDOBJECT_HANDLE), p_initialCapacity); };
inline void Arr_PushBackRealloc_RenderedObjectHandle(ARRAY_REDEREDOBJECT_HANDLE_PTR p_array, RENDEREDOBJECT_HANDLE_PTR p_item) { Arr_PushBackRealloc((ARRAY_PTR)p_array, sizeof(RENDEREDOBJECT_HANDLE), (char*)p_item); };
inline void Arr_Free_RenderedObjectHandle(ARRAY_REDEREDOBJECT_HANDLE_PTR p_array) { Arr_Free((ARRAY_PTR)p_array); };
inline void Arr_Erase_RenderedObjectHandle(ARRAY_REDEREDOBJECT_HANDLE_PTR p_array, size_t p_index) { Arr_Erase((ARRAY_PTR)p_array, sizeof(RENDEREDOBJECT_HANDLE), p_index); };