#pragma once

#include "_interface/DataStructures/ARRAY.h"
#include "TransformC_def.h"
#include "v2/_interface/MatrixC_def.h"

void Transform_Alloc(TRANSFORM_PTR p_transform);
void Transform_Free(TRANSFORM_PTR p_transform);
void Transform_AddChild(TRANSFORM_PTR p_transform, TRANSFORM_PTR p_newChild);

void Transform_GetLocalToWorldMatrix(TRANSFORM_PTR p_transform, MATRIX4F_PTR p_out);
void Transform_GetWorldToLocalMatrix(TRANSFORM_PTR p_transform, MATRIX4F_PTR p_out);
void Transform_CalculateMatrixToProjectFromTransformToAnother(TRANSFORM_PTR p_source, TRANSFORM_PTR p_target, MATRIX4F_PTR p_out);

void Transform_SetLocalPosition(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_localPosition);
void Transform_SetLocalRotation(TRANSFORM_PTR p_transform, const QUATERNION4F_PTR p_localRotation);
void Transform_SetLocalScale(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_localScale);
void Transform_SetWorldPosition(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_worldPosition);
void Transform_AddToWorldPosition(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_worldPosition_delta);
void Transform_SetWorldRotation(TRANSFORM_PTR p_transform, const QUATERNION4F_PTR p_worldRotation);
void Transform_SetWorldScale(TRANSFORM_PTR p_transform, const VECTOR3F_PTR p_worldScale);

void Transform_GetWorldPosition(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetWorldRotation(TRANSFORM_PTR p_transform, QUATERNION4F_PTR p_out);
void Transform_GetWorldScaleFactor(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetRight(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetUp(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetForward(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);

inline void Arr_Alloc_TransformPtr(ARRAY_TRANSFORMHANDLE_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc((ARRAY_PTR)p_arr, sizeof(TRANSFORM_PTR), p_initialCapacity); };
inline void Arr_Free_TransformPtr(ARRAY_TRANSFORMHANDLE_PTR p_arr) { Arr_Free((ARRAY_PTR)p_arr); };
inline void Arr_PushBackRealloc_TransformPtr(ARRAY_TRANSFORMHANDLE_PTR p_arr, TRANSFORM_PTR* p_value) { Arr_PushBackRealloc((ARRAY_PTR)p_arr, sizeof(TRANSFORM_PTR), (char*)p_value); };
inline char Arr_Erase_TransformPtr(ARRAY_TRANSFORMHANDLE_PTR p_arr, size_t p_index) { return Arr_Erase((ARRAY_PTR)p_arr, sizeof(TRANSFORM_PTR), p_index); };

inline void Arr_BuildIterator_TransformPtr(ARRAY_TRANSFORMHANDLE_PTR p_arr, ARRAY_TRANSFORMHANDLE_ITERATOR_PTR p_iter) { Arr_BuildIterator((ARRAY_PTR)p_arr, sizeof(TRANSFORM_PTR), (ARRAY_ITERATOR_PTR)p_iter); }
inline bool Iter_MoveNext_TransformPtr(ARRAY_TRANSFORMHANDLE_ITERATOR_PTR p_iter) { return Iter_MoveNext((ARRAY_ITERATOR_PTR)p_iter, sizeof(TRANSFORM_PTR)); }