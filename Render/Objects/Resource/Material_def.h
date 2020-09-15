#pragma once

#include "v2/_interface/VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct Material_TYP
{
	Vector3c BaseColor;
}Material, * Material_PTR;

typedef struct Array_Material_TYP
{
	ARRAY_TYPE_DEFINITION(Material)
}Array_Material, * Array_Material_PTR;

typedef struct Material_HANDLE_TYP
{
	size_t Handle;
}Material_HANDLE, * Material_HANDLE_PTR;
