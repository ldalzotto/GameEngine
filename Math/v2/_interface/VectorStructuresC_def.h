#pragma once

#include "v2/_interface/VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct Array_Vector3f_TYP
{
	ARRAY_TYPE_DEFINITION(Vector3f)
}Array_Vector3f, * Array_Vector3f_PTR;

typedef struct Array_Vector3f_Iterator_TYP
{
	ARRAY_ITERATOR_DEFINITION(Array_Vector3f, Vector3f)
} Array_Vector3f_Iterator, * Array_Vector3f_Iterator_PTR;

typedef struct ARRAY_Vector3c_TYP
{
	ARRAY_TYPE_DEFINITION(Vector3c)
}ARRAY_Vector3c, * ARRAY_Vector3c_PTR;
