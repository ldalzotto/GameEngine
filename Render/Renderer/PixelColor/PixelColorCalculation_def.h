#pragma once

#include "DataStructures/ARRAY_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/ColorC_def.h"

typedef char PerPolgyonPixelColorCalculationType;
#define PerPolgyonPixelColorCalculationType_NONE 0
#define PerPolgyonPixelColorCalculationType_FlatLightning 1

typedef struct FlatShadingPixelCalculation_TYP
{
	Vector4f PolygonFlatNormal;
	Color3f Out_AttenuatedLightColor;
	Color3f Out_AmbientColor;
}FlatShadingPixelCalculation, *FlatShadingPixelCalculation_PTR;

typedef struct Array_FlatShadingPixelCalculation_TYP
{
	ARRAY_TYPE_DEFINITION(FlatShadingPixelCalculation)
}Array_FlatShadingPixelCalculation, * Array_FlatShadingPixelCalculation_PTR;