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

typedef struct PolygonRasterizer_InterpolationFactor_TYP
{
	// Interpolation factors
	float I0, I1, I2;
}PolygonRasterizer_InterpolationFactor, * PolygonRasterizer_InterpolationFactor_PTR;

typedef struct PolygonRasterizerIterator_CommonStructure_TYP
{
	Recti PolygonBoundClip;
	PolygonRasterize_PackedData PackedRasterizerData;
	PolygonRasterizer_InterpolationFactor InterpolationFactors;
	Vector2i RasterizedPixel;
}PolygonRasterizerIterator_CommonStructure, * PolygonRasterizerIterator_CommonStructure_PTR;

typedef struct PolygonRasterizerIterator_TYP
{
	PolygonRasterizerIterator_CommonStructure CommonStructure;
	
	POLYGONRASTERIZER_ITERATOR_STEP CurrentStep;
	Vector2i CurrentPoint;
	// Rasterizer current edge functions
	int e0, e1, e2;

	Vector2i LineBeginPoint;
	int line_e0, line_e1, line_e2;
}PolygonRasterizerIterator, * PolygonRasterizerIterator_PTR;

void PolygonRasterize_Initialize(const Polygon2i_PTR p_polygon, const Recti_PTR p_clip_rect, PolygonRasterizerIterator_PTR out_polygonRasterizerIterator);
POLYGONRASTERIZER_ITERATOR_RETURN_CODE PolygonRasterize_MoveNext_Interpolated(PolygonRasterizerIterator_PTR p_polygonRasterizerIterator);


typedef char POLYGONRASTERIZER_SMART_TYPE;
#define POLYGONRASTERIZER_SMART_TYPE_NOTHING 0
#define POLYGONRASTERIZER_SMART_TYPE_CENTER_FIND 1
#define POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_LEFT 2
#define POLYGONRASTERIZER_SMART_TYPE_CENTER_LINESCAN_RIGHT 3
#define POLYGONRASTERIZER_SMART_TYPE_CENTER_EXIT 4

typedef struct PolygonRasterizeSmartIterator_TYP
{
	PolygonRasterizerIterator_CommonStructure CommonStructure;

	POLYGONRASTERIZER_SMART_TYPE CurrentStep;

	Vector2i CenterScanCursor;
	int Center_e0, Center_e1, Center_e2;

	Vector2i LineScanCursor;
	int Line_e0, Line_e1, Line_e2;
}PolygonRasterizeSmartIterator, * PolygonRasterizeSmartIterator_PTR;

void PolygonRasterizeSmart_Initialize(const Polygon2i_PTR p_polygon, const Recti_PTR p_clip_rect, PolygonRasterizeSmartIterator_PTR out_polygonRasterizerIterator);
POLYGONRASTERIZER_ITERATOR_RETURN_CODE PolygonRasterizeSmart_MoveNext_Interpolated(PolygonRasterizeSmartIterator_PTR out_polygonRasterizerIterator);