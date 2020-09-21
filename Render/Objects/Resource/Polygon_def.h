#pragma once

#include "v2/_interface/VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

#define POLYGON_TYPE_DEFINITION(ElementType) \
union \
{ \
struct \
{ \
	ElementType v1; \
	ElementType v2; \
	ElementType v3; \
}; \
ElementType Points[3]; \
};

typedef struct Polygonf_TYP
{
	POLYGON_TYPE_DEFINITION(float)
}Polygonf, * Polygonf_PTR;

typedef struct Polygon2f_TYP
{
	POLYGON_TYPE_DEFINITION(Vector2f)
}Polygon2f, * Polygon2f_PTR;

typedef struct Polygon2i_TYP
{
	POLYGON_TYPE_DEFINITION(Vector2i)
}Polygon2i, * Polygon2i_PTR;

typedef struct Polygon4f_TYP
{
	POLYGON_TYPE_DEFINITION(Vector4f)
}Polygon4f, * Polygon4f_PTR;

typedef struct Polygon4fPTR_TYP
{
	POLYGON_TYPE_DEFINITION(Vector4f_PTR)
}Polygon4fPTR, * Polygon4fPTR_PTR;

typedef struct PolygonSizeT_TYP
{
	POLYGON_TYPE_DEFINITION(size_t)
}PolygonSizeT, * PolygonSizeT_PTR;