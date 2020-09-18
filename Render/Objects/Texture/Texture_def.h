#pragma once

#include <stdint.h>
#include "v2/_interface/VectorStructuresC_def.h"
#include "v2/_interface/VectorC_def.h"

typedef struct Texture3c_TYP
{
	uint16_t Width, Height;
	ARRAY_Vector3c Pixels;
}Texture3c, * Texture3c_PTR;

typedef struct Texture3c_MemoryCursor_TYP
{
	Texture3c_PTR Texture;
	Vector3c_PTR Current;
	size_t CurrentIndex;
}Texture3c_MemoryCursor, * Texture3c_MemoryCursor_PTR;
