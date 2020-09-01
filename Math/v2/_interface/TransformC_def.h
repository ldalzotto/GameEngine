#pragma once

#include <stdbool.h>
#include "VectorC_def.h"
#include "QuaternionC_def.h"
#include "MatrixC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct Transform_TYP* Transform_PTR;

typedef struct Array_TransformHandle_TYP
{
	ARRAY_TYPE_DEFINITION(Transform_PTR)
}  Array_TransformHandle, * Array_TransformHandle_PTR;

typedef struct Transform_TYP
{
	Transform_PTR Parent;
	Array_TransformHandle Childs;

	bool MatricesMustBeRecalculated;
	bool UserFlag_HasChanged;

	Vector3f LocalPosition;
	Quaternion4f LocalRotation;
	Vector3f LocalScale;

	/** This matrix will always be relative to the top level Transform (a Transform without parent). */
	Matrix4f LocalToWorldMatrix;

} Transform, * Transform_PTR;
