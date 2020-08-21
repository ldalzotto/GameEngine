#pragma once

#include "v2/_interface/VectorC_def.h"

typedef struct Polygon2f_TYP
{
	Vector2f v1;
	Vector2f v2;
	Vector2f v3;
}Polygon2f, * Polygon2f_PTR;

typedef struct Polygon4f_TYP
{
	Vector4f v1;
	Vector4f v2;
	Vector4f v3;
}Polygon4f, * Polygon4f_PTR;

#define POLYGON_TYPE_DEFINITION(ElementType) \
ElementType v1; \
ElementType v2; \
ElementType v3;
