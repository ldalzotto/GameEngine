#pragma once

#include <stdint.h>
#include "DataStructures/ARRAY_def.h"
#include "v2/_interface/VectorStructuresC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/ColorC_def.h"
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/WindowSize_def.h"

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

typedef struct Array_Texutre3cHandle_TYP
{
	ARRAY_TYPE_DEFINITION(Texture3c_HANDLE)
}Array_Texutre3cHandle, * Array_Texutre3cHandle_PTR;

typedef struct Texture3c_MemoryCursor_TYP
{
	Texture3c_PTR Texture;
	Color3c_PTR Current;
	size_t CurrentIndex;
}Texture3c_MemoryCursor, * Texture3c_MemoryCursor_PTR;


typedef struct Texture3f_TYP
{
	uint16_t Width, Height;
	Array_Color3f Pixels;
}Texture3f, * Texture3f_PTR;

typedef struct Texturef_TYP
{
	uint16_t Width, Height;
	Array_Float Pixels;
}Texturef, * Texturef_PTR;

typedef struct RenderTexture3f_TYP
{
	Texture3f Texture;
	Recti BoundingRectangle;
	WindowSize PrecalculatedDimensions;
}RenderTexture3f, *RenderTexture3f_PTR;