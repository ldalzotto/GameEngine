#pragma once

#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "Objects/Resource/Mesh_def.h"
#include "Objects/Resource/Material_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct RenderedObject_TYP
{
	Mesh_PTR Mesh;
	BoxF_PTR MeshBoundingBox;
	Matrix4f ModelMatrix;
	Material_HANDLE Material;
	char IsCulled;
}RenderedObject, * RenderedObject_PTR;

typedef struct Array_RenderedObject_TYP
{
	ARRAY_TYPE_DEFINITION(RenderedObject)
}Array_RenderedObject, * Array_RenderedObject_PTR;

typedef struct RenderedObject_HANDLE_TYP
{
	size_t Handle;
}RenderedObject_HANDLE, * RenderedObject_HANDLE_PTR;

typedef struct Array_RenderedObjectHandle_TYP
{
	ARRAY_TYPE_DEFINITION(RenderedObject_HANDLE)
}Array_RenderedObjectHandle, * Array_RenderedObjectHandle_PTR;

