#pragma once

#include <stdint.h>
#include "v2/_interface/VectorStructuresC_def.h"
#include "v2/_interface/VectorC_def.h"

typedef struct TEXTURE3C_TYP
{
	uint16_t Width, Height;
	ARRAY_VECTOR3C Pixels;
}TEXTURE3C, * TEXTURE3C_PTR;

typedef struct TEXTURE3C_MEMORYCURSOR_TYP
{
	TEXTURE3C_PTR Texture;
	VECTOR3C_PTR Current;
	size_t CurrentIndex;
}TEXTURE3C_MEMORYCURSOR, * TEXTURE3C_MEMORYCURSOR_PTR;

