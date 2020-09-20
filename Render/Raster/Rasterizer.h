#pragma once
#pragma once

#include <stdbool.h>
#include "Objects/Resource/Polygon_def.h"
#include "v2/_interface/RectC_def.h"

typedef char LINERASTERIZER_ITERATOR_STEP;
#define LINERASTERIZER_ITERATOR_STEP_BEGIN 0
#define LINERASTERIZER_ITERATOR_STEP_LOOPING 1
#define LINERASTERIZER_ITERATOR_STEP_END 2

typedef struct LineRasterizerIterator_TYP
{
	Vector2i CurrentPoint;
	Vector2i EndPoint;
	int dx, dy, sx, sy, err;
	LINERASTERIZER_ITERATOR_STEP CurrentStep;
}LineRasterizerIterator, * LineRasterizerIterator_PTR;

char LineRasterize_Initialize(const Vector2i_PTR p_begin, const Vector2i_PTR p_end, const Recti_PTR p_clip_rect, LineRasterizerIterator_PTR out_lineRasterizerIterator);
char LineRasterize_MoveNext(LineRasterizerIterator_PTR out_lineRasterizerIterator);

#if 0
void Rasterize_PolygonClipped(const Polygon2i_PTR p_polygon, Array_Vector2i_PTR out_rasterizedPixels, const Recti_PTR p_clip_rect);
#endif

typedef char POLYGONRASTERIZER_ITERATOR_STEP;
#define POLYGONRASTERIZER_ITERATOR_STEP_NOTHING 0
#define POLYGONRASTERIZER_ITERATOR_STEP_NEWLINE 1
#define POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN 2
#define POLYGONRASTERIZER_ITERATOR_STEP_EXIT 3

typedef char POLYGONRASTERIZER_ITERATOR_RETURN_CODE;
#define POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END 0
#define POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED 1
#define POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED 2

typedef struct PolygonRasterize_PackedData_TYP
{
	int dx0, dy0, dx1, dy1, dx2, dy2;

	// Interpolation precalculated areas
	// areaX means the area proportional to the interpolation IX.
	int area0, area1, area2;
}PolygonRasterize_PackedData, * PolygonRasterize_PackedData_PTR;

typedef struct PolygonRasterizerIterator_TYP
{
	char InterpolationEnabled;
	POLYGONRASTERIZER_ITERATOR_STEP CurrentStep;
	Vector2i CurrentPoint;
	Recti PolygonBoundClip;
	int LineIndexCursor, ColumnIndexCursor;
	PolygonRasterize_PackedData PackedRasterizerData;
	// Rasterizer edge functions
	int e0, e1, e2;
	// Interpolation factors
	float I0, I1, I2;

	Vector2i RasterizedPixel;
}PolygonRasterizerIterator, * PolygonRasterizerIterator_PTR;

void PolygonRasterize_Initialize(const Polygon2i_PTR p_polygon, const Recti_PTR p_clip_rect, char enableInterpolation, PolygonRasterizerIterator_PTR out_polygonRasterizerIterator);
POLYGONRASTERIZER_ITERATOR_RETURN_CODE PolygonRasterize_MoveNext(PolygonRasterizerIterator_PTR p_polygonRasterizerIterator);
