#pragma once

#include <stdint.h>
#include "DataStructures/ARRAY_def.h"
#include "v2/_interface/VectorStructuresC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/ColorC_def.h"

typedef struct Texture3c_TYP
{
	uint16_t Width, Height;
	Array_Color3c Pixels;
}Texture3c, * Texture3c_PTR;

typedef struct Array_Texture3c_TYP
{
	ARRAY_TYPE_DEFINITION(Texture3c)
}Array_Texture3c, * Array_Texture3c_PTR;

typedef struct Texture3c_HANDLE_TYP
{
	size_t Handle;
}Texture3c_HANDLE, * Texture3c_HANDLE_PTR;

typedef struct Texture3c_MemoryCursor_TYP
{
	Texture3c_PTR Texture;
	Color3c_PTR Current;
	size_t CurrentIndex;
}Texture3c_MemoryCursor, * Texture3c_MemoryCursor_PTR;
