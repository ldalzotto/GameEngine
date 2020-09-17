#include "Rasterizer.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "Constants.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/VectorStructuresC.h"
#include "v2/_interface/VectorC.h"
#include "v2/_interface/RectC.h"
#include "Error/ErrorHandler.h"


// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
char LineRasterize_Initialize(const Vector2i_PTR p_begin, const Vector2i_PTR p_end, const Recti_PTR p_clip_rect, LineRasterizerIterator_PTR out_lineRasterizerIterator)
{
	Vector2i l_clipped_begin, l_clipped_end;
	if (Rect_ClipSegment_Int(p_begin, p_end, p_clip_rect, &l_clipped_begin, &l_clipped_end))
	{
		out_lineRasterizerIterator->CurrentPoint = l_clipped_begin;
		out_lineRasterizerIterator->EndPoint = l_clipped_end;
		out_lineRasterizerIterator->dx = abs(l_clipped_end.x - l_clipped_begin.x); out_lineRasterizerIterator->sx = l_clipped_begin.x < l_clipped_end.x ? 1 : -1;
		out_lineRasterizerIterator->dy = -abs(l_clipped_end.y - l_clipped_begin.y); out_lineRasterizerIterator->sy = l_clipped_begin.y < l_clipped_end.y ? 1 : -1;
		out_lineRasterizerIterator->err = out_lineRasterizerIterator->dx + out_lineRasterizerIterator->dy;
		out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_BEGIN;
		return 1;
	};
	return 0;
};

char LineRasterize_MoveNext(LineRasterizerIterator_PTR out_lineRasterizerIterator)
{
	switch (out_lineRasterizerIterator->CurrentStep)
	{
	case LINERASTERIZER_ITERATOR_STEP_BEGIN:
	{
		out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_LOOPING;
		return 1;
	}
	break;
	case LINERASTERIZER_ITERATOR_STEP_LOOPING:
	{
		int e2 = 2 * out_lineRasterizerIterator->err;
		if (e2 >= out_lineRasterizerIterator->dy)
		{
			if (out_lineRasterizerIterator->CurrentPoint.x == out_lineRasterizerIterator->EndPoint.x)
			{
				out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_END;
				return 1;
			}
			out_lineRasterizerIterator->err += out_lineRasterizerIterator->dy;
			out_lineRasterizerIterator->CurrentPoint.x += out_lineRasterizerIterator->sx;
		}
		if (e2 <= out_lineRasterizerIterator->dx)
		{
			if (out_lineRasterizerIterator->CurrentPoint.y == out_lineRasterizerIterator->EndPoint.y)
			{
				out_lineRasterizerIterator->CurrentStep = LINERASTERIZER_ITERATOR_STEP_END;
				return 1;
			}
			out_lineRasterizerIterator->err += out_lineRasterizerIterator->dx;
			out_lineRasterizerIterator->CurrentPoint.y += out_lineRasterizerIterator->sy;
		}
		return 1;
	}
	break;
	case LINERASTERIZER_ITERATOR_STEP_END:
	{
		return 0;
	}
	break;
	}

	return 0;
};

inline int PolygonRasterizePackedData_EdgeFunction_01(const PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon, const Vector2i_PTR p_point)
{
	return ((p_point->x - p_polygon->v2.x) * p_packedData->dy0) - ((p_point->y - p_polygon->v2.y) * p_packedData->dx0);
};

inline int PolygonRasterizePackedData_EdgeFunction_12(const PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon, const Vector2i_PTR p_point)
{
	return ((p_point->x - p_polygon->v3.x) * p_packedData->dy1) - ((p_point->y - p_polygon->v3.y) * p_packedData->dx1);
};

inline int PolygonRasterizePackedData_EdgeFunction_20(const PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon, const Vector2i_PTR p_point)
{
	return ((p_point->x - p_polygon->v1.x) * p_packedData->dy2) - ((p_point->y - p_polygon->v1.y) * p_packedData->dx2);
};


void PolygonRasterizePackedData_PrecalculateRasterization(PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon)
{
	p_packedData->dx0 = p_polygon->v2.x - p_polygon->v1.x;
	p_packedData->dy0 = p_polygon->v2.y - p_polygon->v1.y;

	p_packedData->dx1 = p_polygon->v3.x - p_polygon->v2.x;
	p_packedData->dy1 = p_polygon->v3.y - p_polygon->v2.y;

	p_packedData->dx2 = p_polygon->v1.x - p_polygon->v3.x;
	p_packedData->dy2 = p_polygon->v1.y - p_polygon->v3.y;

	Vector3f l_begin = { (float)p_polygon->v2.x - p_polygon->v1.x, (float)p_polygon->v2.y - p_polygon->v1.y, 0.0f };
	Vector3f l_end = { (float)p_polygon->v3.x - p_polygon->v1.x, (float)p_polygon->v3.y - p_polygon->v1.y, 0.0f };
	Vector3f l_crossResult;
	Vec_Cross_3f(&l_begin, &l_end, &l_crossResult);

	// Anti-clockwise
	if (l_crossResult.z > 0.0f)
	{
		p_packedData->dx0 *= -1;
		p_packedData->dy0 *= -1;
		p_packedData->dx1 *= -1;
		p_packedData->dy1 *= -1;
		p_packedData->dx2 *= -1;
		p_packedData->dy2 *= -1;
	}
};

void PolygonRasterizePackedData_PrecalculateInterpolation(PolygonRasterize_PackedData_PTR p_packedData, const Polygon2i_PTR p_polygon)
{
	p_packedData->area0 = PolygonRasterizePackedData_EdgeFunction_12(p_packedData, p_polygon, &p_polygon->v1);
	p_packedData->area1 = PolygonRasterizePackedData_EdgeFunction_20(p_packedData, p_polygon, &p_polygon->v2);
	p_packedData->area2 = PolygonRasterizePackedData_EdgeFunction_01(p_packedData, p_polygon, &p_polygon->v3);
};

void PolygonRasterize_Initialize(const Polygon2i_PTR p_polygon, const Recti_PTR p_clip_rect, char enableInterpolation, PolygonRasterizerIterator_PTR out_polygonRasterizerIterator)
{
	out_polygonRasterizerIterator->InterpolationEnabled = enableInterpolation;

	//Calculate polygon rect bound clipped
	Polygon_BoundingRect_2i(p_polygon, &out_polygonRasterizerIterator->PolygonBoundClip);
	Rect_ClipPoint_Int(p_clip_rect, &out_polygonRasterizerIterator->PolygonBoundClip.Min);
	Rect_ClipPoint_Int(p_clip_rect, &out_polygonRasterizerIterator->PolygonBoundClip.Max);

	out_polygonRasterizerIterator->CurrentPoint = out_polygonRasterizerIterator->PolygonBoundClip.Min;
	PolygonRasterizePackedData_PrecalculateRasterization(&out_polygonRasterizerIterator->PackedRasterizerData, p_polygon);
	out_polygonRasterizerIterator->e0 = PolygonRasterizePackedData_EdgeFunction_01(&out_polygonRasterizerIterator->PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CurrentPoint);
	out_polygonRasterizerIterator->e1 = PolygonRasterizePackedData_EdgeFunction_12(&out_polygonRasterizerIterator->PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CurrentPoint);
	out_polygonRasterizerIterator->e2 = PolygonRasterizePackedData_EdgeFunction_20(&out_polygonRasterizerIterator->PackedRasterizerData, p_polygon, &out_polygonRasterizerIterator->CurrentPoint);

	if (out_polygonRasterizerIterator->InterpolationEnabled)
	{
		PolygonRasterizePackedData_PrecalculateInterpolation(&out_polygonRasterizerIterator->PackedRasterizerData, p_polygon);
	}

	out_polygonRasterizerIterator->LineIndexCursor = out_polygonRasterizerIterator->PolygonBoundClip.Min.y;
	out_polygonRasterizerIterator->ColumnIndexCursor = out_polygonRasterizerIterator->PolygonBoundClip.Min.x - 1;
	out_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN;
}

// For interpolation, we can use e1, e2, e0 as they represent the already calculated edge functions for the current pixel
void PolygonRasterize_Interpolate(PolygonRasterizerIterator_PTR p_polygonRasterizerIterator)
{ 
	p_polygonRasterizerIterator->I0 = ((float)p_polygonRasterizerIterator->e1 / (float)p_polygonRasterizerIterator->PackedRasterizerData.area0);
	p_polygonRasterizerIterator->I1 = ((float)p_polygonRasterizerIterator->e2 / (float)p_polygonRasterizerIterator->PackedRasterizerData.area1);
	p_polygonRasterizerIterator->I2 = ((float)p_polygonRasterizerIterator->e0 / (float)p_polygonRasterizerIterator->PackedRasterizerData.area2);
};

//A Parallel Algorithm for Polygon Rasterization.pdf
//TODO -> Highly unoptimized, we iterate over the whole polygon bound clip.
// We can easily implement a smart iteration that try to navigates inside the polygon and try to find edges (thus, ignoring calcuation of empty )
// if in the future, we want to parrallelize the rasteriation calculation, we can precalculatate e0, e1, and e2 for every pixels of the bound rectangle as the formula is linear
POLYGONRASTERIZER_ITERATOR_RETURN_CODE PolygonRasterize_MoveNext(PolygonRasterizerIterator_PTR p_polygonRasterizerIterator)
{
	switch (p_polygonRasterizerIterator->CurrentStep)
	{
	case POLYGONRASTERIZER_ITERATOR_STEP_NEWLINE:
	{
		// For loop on p_polygonRasterizerIterator->LineIndexCursor for line from p_polygonRasterizerIterator->PolygonBoundClip.Min.y to p_polygonRasterizerIterator->PolygonBoundClip.Max.y (included)
		p_polygonRasterizerIterator->LineIndexCursor += 1;
		if (p_polygonRasterizerIterator->LineIndexCursor > p_polygonRasterizerIterator->PolygonBoundClip.Max.y)
		{
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NOTHING;
			return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END;
		}

		POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;

		if (p_polygonRasterizerIterator->e0 >= 0 && p_polygonRasterizerIterator->e1 >= 0 && p_polygonRasterizerIterator->e2 >= 0)
		{
			l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
			p_polygonRasterizerIterator->RasterizedPixel = p_polygonRasterizerIterator->CurrentPoint;

			if (p_polygonRasterizerIterator->InterpolationEnabled)
			{
				PolygonRasterize_Interpolate(p_polygonRasterizerIterator);
			}
		}

		p_polygonRasterizerIterator->ColumnIndexCursor = p_polygonRasterizerIterator->PolygonBoundClip.Min.x - 1;
		p_polygonRasterizerIterator->CurrentPoint.x = p_polygonRasterizerIterator->PolygonBoundClip.Min.x;

		p_polygonRasterizerIterator->e0 -= ((p_polygonRasterizerIterator->PolygonBoundClip.Max.x - p_polygonRasterizerIterator->PolygonBoundClip.Min.x) * p_polygonRasterizerIterator->PackedRasterizerData.dy0);
		p_polygonRasterizerIterator->e1 -= ((p_polygonRasterizerIterator->PolygonBoundClip.Max.x - p_polygonRasterizerIterator->PolygonBoundClip.Min.x) * p_polygonRasterizerIterator->PackedRasterizerData.dy1);
		p_polygonRasterizerIterator->e2 -= ((p_polygonRasterizerIterator->PolygonBoundClip.Max.x - p_polygonRasterizerIterator->PolygonBoundClip.Min.x) * p_polygonRasterizerIterator->PackedRasterizerData.dy2);

		p_polygonRasterizerIterator->CurrentPoint.y = p_polygonRasterizerIterator->LineIndexCursor;

		p_polygonRasterizerIterator->e0 -= p_polygonRasterizerIterator->PackedRasterizerData.dx0;
		p_polygonRasterizerIterator->e1 -= p_polygonRasterizerIterator->PackedRasterizerData.dx1;
		p_polygonRasterizerIterator->e2 -= p_polygonRasterizerIterator->PackedRasterizerData.dx2;

		p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN;
		return l_returnCode;

	}
	break;
	case POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN:
	{
		// For loop on p_polygonRasterizerIterator->ColumnIndexCursor for colum from p_polygonRasterizerIterator->PolygonBoundClip.Min.x to p_polygonRasterizerIterator->PolygonBoundClip.Max.x (excluded)
		p_polygonRasterizerIterator->ColumnIndexCursor += 1;
		if (p_polygonRasterizerIterator->ColumnIndexCursor >= p_polygonRasterizerIterator->PolygonBoundClip.Max.x)
		{
			p_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWLINE;
			return PolygonRasterize_MoveNext(p_polygonRasterizerIterator);
		}

		POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;

		if (p_polygonRasterizerIterator->e0 >= 0 && p_polygonRasterizerIterator->e1 >= 0 && p_polygonRasterizerIterator->e2 >= 0)
		{
			l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED;
			p_polygonRasterizerIterator->RasterizedPixel = p_polygonRasterizerIterator->CurrentPoint;

			if (p_polygonRasterizerIterator->InterpolationEnabled)
			{
				PolygonRasterize_Interpolate(p_polygonRasterizerIterator);
			}
		}

		p_polygonRasterizerIterator->CurrentPoint.x = p_polygonRasterizerIterator->ColumnIndexCursor + 1;

		p_polygonRasterizerIterator->e0 += p_polygonRasterizerIterator->PackedRasterizerData.dy0;
		p_polygonRasterizerIterator->e1 += p_polygonRasterizerIterator->PackedRasterizerData.dy1;
		p_polygonRasterizerIterator->e2 += p_polygonRasterizerIterator->PackedRasterizerData.dy2;

		return l_returnCode;
	}
	break;
	}

	return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END;
};
