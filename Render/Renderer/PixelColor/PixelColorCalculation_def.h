#pragma once

#include "DataStructures/ARRAY_def.h"

typedef char PerPolgyonPixelColorCalculationType;
#define PerPolgyonPixelColorCalculationType_NONE 0
#define PerPolgyonPixelColorCalculationType_FlatLightning 1

typedef struct FlatShadingPixelCalculation_TYP
{
	size_t PolygonVaryings;
	float Out_DotProduct;
}FlatShadingPixelCalculation, *FlatShadingPixelCalculation_PTR;

typedef struct Array_FlatShadingPixelCalculation_TYP
{
	ARRAY_TYPE_DEFINITION(FlatShadingPixelCalculation)
}Array_FlatShadingPixelCalculation, * Array_FlatShadingPixelCalculation_PTR;