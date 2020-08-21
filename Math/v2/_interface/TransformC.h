#pragma once

#include "DataStructures/ARRAY.h"
#include "TransformC_def.h"
#include "v2/_interface/MatrixC_def.h"

void Transform_Alloc(Transform_PTR p_transform);
void Transform_Free(Transform_PTR p_transform);
void Transform_AddChild(Transform_PTR p_transform, Transform_PTR p_newChild);

void Transform_GetLocalToWorldMatrix(Transform_PTR p_transform, Matrix4f_PTR p_out);
void Transform_GetWorldToLocalMatrix(Transform_PTR p_transform, Matrix4f_PTR p_out);
void Transform_CalculateMatrixToProjectFromTransformToAnother(Transform_PTR p_source, Transform_PTR p_target, Matrix4f_PTR p_out);

void Transform_SetLocalPosition(Transform_PTR p_transform, const Vector3f_PTR p_localPosition);
void Transform_SetLocalRotation(Transform_PTR p_transform, const Quaternion4f_PTR p_localRotation);
void Transform_SetLocalScale(Transform_PTR p_transform, const Vector3f_PTR p_localScale);
void Transform_SetWorldPosition(Transform_PTR p_transform, const Vector3f_PTR p_worldPosition);
void Transform_AddToWorldPosition(Transform_PTR p_transform, const Vector3f_PTR p_worldPosition_delta);
void Transform_SetWorldRotation(Transform_PTR p_transform, const Quaternion4f_PTR p_worldRotation);
void Transform_SetWorldScale(Transform_PTR p_transform, const Vector3f_PTR p_worldScale);

void Transform_GetWorldPosition(Transform_PTR p_transform, Vector3f_PTR p_out);
void Transform_GetWorldRotation(Transform_PTR p_transform, Quaternion4f_PTR p_out);
void Transform_GetWorldScaleFactor(Transform_PTR p_transform, Vector3f_PTR p_out);
void Transform_GetRight(Transform_PTR p_transform, Vector3f_PTR p_out);
void Transform_GetUp(Transform_PTR p_transform, Vector3f_PTR p_out);
void Transform_GetForward(Transform_PTR p_transform, Vector3f_PTR p_out);

inline void Arr_Alloc_TransformPtr(Array_TransformHandle_PTR p_arr, size_t p_initialCapacity) { Arr_Alloc((Array_PTR)p_arr, sizeof(Transform_PTR), p_initialCapacity); };
inline void Arr_Free_TransformPtr(Array_TransformHandle_PTR p_arr) { Arr_Free((Array_PTR)p_arr); };
inline char Arr_PushBackRealloc_TransformPtr(Array_TransformHandle_PTR p_arr, Transform_PTR* p_value) { return Arr_PushBackRealloc((Array_PTR)p_arr, sizeof(Transform_PTR), (char*)p_value); };
inline char Arr_Erase_TransformPtr(Array_TransformHandle_PTR p_arr, size_t p_index) { return Arr_Erase((Array_PTR)p_arr, sizeof(Transform_PTR), p_index); };

inline void Arr_BuildIterator_TransformPtr(Array_TransformHandle_PTR p_arr, Array_TransformHandle_Iterator_PTR p_iter) { Arr_BuildIterator((Array_PTR)p_arr, sizeof(Transform_PTR), (ArrayIterator_PTR)p_iter); }
inline bool Iter_MoveNext_TransformPtr(Array_TransformHandle_Iterator_PTR p_iter) { return Iter_MoveNext((ArrayIterator_PTR)p_iter, sizeof(Transform_PTR)); }