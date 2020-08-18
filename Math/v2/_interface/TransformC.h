#pragma once

#include "TransformC_def.h"
#include "v2/_interface/MatrixC_def.h"

void Transform_Alloc(TRANSFORM_PTR p_transform);
void Transform_Free(TRANSFORM_PTR p_transform);
void Transform_AddChild(TRANSFORM_PTR p_transform, TRANSFORM_PTR p_newChild);

void Transform_GetLocalToWorldMatrix(TRANSFORM_PTR p_transform, MATRIX4F_PTR p_out);
void Transform_GetWorldToLocalMatrix(TRANSFORM_PTR p_transform, MATRIX4F_PTR p_out);
void Transform_CalculateMatrixToProjectFromTransformToAnother(TRANSFORM_PTR p_source, TRANSFORM_PTR p_target, MATRIX4F_PTR p_out);
void Transform_GetWorldPosition(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetWorldRotation(TRANSFORM_PTR p_transform, QUATERNION4F_PTR p_out);
void Transform_GetWorldScaleFactor(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetRight(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetUp(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);
void Transform_GetForward(TRANSFORM_PTR p_transform, VECTOR3F_PTR p_out);