#pragma once

#include "Rasterizer_def.h"
#include <stdbool.h>
#include "DataStructures/ARRAY.h"
#include "Error/ErrorHandler.h"
#include "Objects/Resource/Polygon_def.h"
#include "v2/_interface/VectorStructuresC_def.h"
#include "v2/_interface/RectC_def.h"


bool Rasterize_LineClipped(
	const Vector2i_PTR p_begin, const Vector2i_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR out_rasterizedPixels,
	const Recti_PTR p_clip_rect,
	Vector2i_PTR out_clipped_begin,
	Vector2i_PTR out_clipped_end);

#if 0
void Rasterize_PolygonClipped(const Polygon2i_PTR p_polygon, Array_Vector2i_PTR out_rasterizedPixels, const Recti_PTR p_clip_rect);
#endif

typedef char POLYGONRASTERIZER_ITERATOR_STEP;
#define POLYGONRASTERIZER_ITERATOR_STEP_NOTHING 0
#define POLYGONRASTERIZER_ITERATOR_STEP_NEWLINE 1
#define POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN 2

typedef char POLYGONRASTERIZER_ITERATOR_RETURN_CODE;
#define POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END 0
#define POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED 1
#define POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED 2

typedef struct PolygonRasterizerIterator_TYP
{
	POLYGONRASTERIZER_ITERATOR_STEP CurrentStep;
	Vector2i CurrentPoint;
	Recti PolygonBoundClip;
	int dx0, dy0, e0, dx1, dy1, e1, dx2, dy2, e2;
	int LineIndexCursor, ColumnIndexCursor;
	Vector2i RasterizedPixel;
}PolygonRasterizerIterator, * PolygonRasterizerIterator_PTR;

void PolygonRasterize_Initialize(const Polygon2i_PTR in_out_polygon, const Recti_PTR p_clip_rect, PolygonRasterizerIterator_PTR out_polygonRasterizerIterator);
POLYGONRASTERIZER_ITERATOR_RETURN_CODE PolygonRasterize_MoveNext(PolygonRasterizerIterator_PTR p_polygonRasterizerIterator);

inline void Arr_Alloc_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array, size_t p_initialSize) { Arr_Alloc(&p_array->array, sizeof(RASTERIZATIONSTEP), p_initialSize); }
inline void Arr_Free_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array) { Arr_Free(&p_array->array); }

inline void Arr_Clear_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array) { Arr_Clear(&p_array->array); };
inline void Arr_Resize_RasterisationStep(ARRAY_RASTERISATIONSTEP_PTR p_array, size_t p_newCapacity) { HANDLE_ERR(Arr_Resize(&p_array->array, sizeof(RASTERIZATIONSTEP), p_newCapacity)); };