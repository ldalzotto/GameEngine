#pragma once

#include "v2/_interface/ColorC_def.h"
#include "DataStructures/ARRAY_def.h"
#include "Objects/Texture/Texture_def.h"

typedef unsigned char MATERIAL_TYPE;
#define MATERIAL_TYPE_NotShaded_NotTextured 0
#define MATERIAL_TYPE_NotShaded_Textured 1
#define MATERIAL_TYPE_FlatShaded_Textured 2
#define MATERIAL_TYPE_FlatShaded_NotTextured 3

typedef struct Material_TYP
{
	MATERIAL_TYPE Type;
	Color3f BaseColor;
	Texture3c_HANDLE DiffuseTexture;
}Material, * Material_PTR;

typedef struct Array_Material_TYP
{
	ARRAY_TYPE_DEFINITION(Material)
}Array_Material, * Array_Material_PTR;

typedef struct Material_HANDLE_TYP
{
	size_t Handle;
}Material_HANDLE, * Material_HANDLE_PTR;
