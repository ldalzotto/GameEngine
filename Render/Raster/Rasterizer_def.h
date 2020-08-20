#pragma once


#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef short int RasterizationStepDirection;
#define RasterizationStepDirection_NONE 0
#define RasterizationStepDirection_ADD 1
#define RasterizationStepDirection_REMOVE 2
 
typedef struct RASTERIZATIONSTEP_TYP
{
	RasterizationStepDirection XDirection;
	RasterizationStepDirection YDirection;
}RASTERIZATIONSTEP, * RASTERIZATIONSTEP_PTR;

typedef struct ARRAY_RASTERISATIONSTEP_TYP
{
	ARRAY_TYPE_DEFINITION(RASTERIZATIONSTEP)
}ARRAY_RASTERISATIONSTEP, * ARRAY_RASTERISATIONSTEP_PTR;

