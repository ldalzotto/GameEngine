#pragma once

#include "v2/_interface/VectorC_def.h"

typedef struct POLYGON2F_TYP
{
	VECTOR2F v1;
	VECTOR2F v2;
	VECTOR2F v3;
}POLYGON2F, * POLYGON2F_PTR;

typedef struct POLYGON4F_TYP
{
	VECTOR4F v1;
	VECTOR4F v2;
	VECTOR4F v3;
}POLYGON4F, * POLYGON4F_PTR;

#define POLYGON_TYPE_DEFINITION(ElementType) \
ElementType v1; \
ElementType v2; \
ElementType v3;
