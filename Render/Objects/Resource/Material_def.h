#pragma once

#include "v2/_interface/ColorC_def.h"
#include "DataStructures/ARRAY_def.h"
#include "Objects/Texture/Texture_def.h"

typedef char MATERIAL_SHADING_TYPE;
#define MATERIAL_SHADING_TYPE_NONE 0
#define MATERIAL_SHADING_TYPE_FLAT 1

typedef char MATERIAL_MESHPROPERTY_USAGE;
#define MATERIAL_MESHPROPERTY_USAGE_NONE 0
#define MATERIAL_MESHPROPERTY_USAGE_UV 1

typedef struct Material_TYP
{
	MATERIAL_SHADING_TYPE ShadingType;
	MATERIAL_MESHPROPERTY_USAGE MeshPropertyUsage;
	
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
