#pragma once

#include "v2/_interface/VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct Polygon2f_TYP
{
	Vector2f v1;
	Vector2f v2;
	Vector2f v3;
}Polygon2f, * Polygon2f_PTR;

typedef struct Polygon2i_TYP
{
	union
	{
		struct
		{
			Vector2i v1;
			Vector2i v2;
			Vector2i v3;
		};
		Vector2i Points[3];
	};
}Polygon2i, * Polygon2i_PTR;

typedef struct Polygon4f_TYP
{
	Vector4f v1;
	Vector4f v2;
	Vector4f v3;
}Polygon4f, * Polygon4f_PTR;

typedef struct Polygon4fPTR_TYP
{
	Vector4f_PTR v1;
	Vector4f_PTR v2;
	Vector4f_PTR v3;
}Polygon4fPTR, * Polygon4fPTR_PTR;

#define POLYGON_TYPE_DEFINITION(ElementType) \
ElementType v1; \
ElementType v2; \
ElementType v3;


typedef struct PrecaculatedPolygonFlatNormal_TYP
{
	Vector4f LocalNormal;
}PrecaculatedPolygonFlatNormal, * PrecaculatedPolygonFlatNormal_PTR;

typedef struct Array_PrecaculatedPolygonFlatNormal_TYP
{
	ARRAY_TYPE_DEFINITION(PrecaculatedPolygonFlatNormal)
}Array_PrecaculatedPolygonFlatNormal, * Array_PrecaculatedPolygonFlatNormal_PTR;

typedef struct PrecaculatedPolygonFlatNormal_HANDLE_TYP
{
	size_t Handle;
}PrecaculatedPolygonFlatNormal_HANDLE, * PrecaculatedPolygonFlatNormal_HANDLE_PTR;

typedef struct Array_PrecaculatedPolygonFlatNormalHANDLE_TYP
{
	ARRAY_TYPE_DEFINITION(PrecaculatedPolygonFlatNormal_HANDLE)
}Array_PrecaculatedPolygonFlatNormalHANDLE, * Array_PrecaculatedPolygonFlatNormalHANDLE_PTR;