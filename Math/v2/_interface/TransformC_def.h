#pragma once

#include <stdbool.h>
#include "VectorC_def.h"
#include "QuaternionC_def.h"
#include "MatrixC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct TRANSFORM_TYP* TRANSFORM_PTR;

typedef struct ARRAY_TRANSFORMHANDLE_TYP
{
	ARRAY_TYPE_DEFINITION(TRANSFORM_PTR)
}  ARRAY_TRANSFORMHANDLE, * ARRAY_TRANSFORMHANDLE_PTR;

typedef struct ARRAY_TRANSFORMHANDLE_ITERATOR_TYP
{
	ARRAY_ITERATOR_DEFINITION(ARRAY_TRANSFORMHANDLE, TRANSFORM_PTR)
} ARRAY_TRANSFORMHANDLE_ITERATOR, * ARRAY_TRANSFORMHANDLE_ITERATOR_PTR;

typedef struct TRANSFORM_TYP
{
	TRANSFORM_PTR Parent;
	ARRAY_TRANSFORMHANDLE Childs;

	bool MatricesMustBeRecalculated;
	bool UserFlag_HasChanged;

	VECTOR3F LocalPosition;
	QUATERNION4F LocalRotation;
	VECTOR3F LocalScale;

	/** This matrix will always be relative to the top level Transform (a Transform without parent). */
	MATRIX4F LocalToWorldMatrix;

} TRANSFORM, * TRANSFORM_PTR;
