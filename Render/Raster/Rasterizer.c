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

void PolygonRasterize_Initialize(const Polygon2i_PTR in_out_polygon, const Recti_PTR p_clip_rect, PolygonRasterizerIterator_PTR out_polygonRasterizerIterator)
{
	//Calculate polygon rect bound clipped
	Polygon_BoundingRect_2i(in_out_polygon, &out_polygonRasterizerIterator->PolygonBoundClip);
	Rect_ClipPoint_Int(p_clip_rect, &out_polygonRasterizerIterator->PolygonBoundClip.Min);
	Rect_ClipPoint_Int(p_clip_rect, &out_polygonRasterizerIterator->PolygonBoundClip.Max);

	out_polygonRasterizerIterator->CurrentPoint = out_polygonRasterizerIterator->PolygonBoundClip.Min;

	//Line 1
	out_polygonRasterizerIterator->dx0 = in_out_polygon->v2.x - in_out_polygon->v1.x;
	out_polygonRasterizerIterator->dy0 = in_out_polygon->v2.y - in_out_polygon->v1.y;
	out_polygonRasterizerIterator->e0 = ((out_polygonRasterizerIterator->CurrentPoint.x - in_out_polygon->v2.x) * out_polygonRasterizerIterator->dy0) - ((out_polygonRasterizerIterator->CurrentPoint.y - in_out_polygon->v2.y) * out_polygonRasterizerIterator->dx0);

	//Line 2
	out_polygonRasterizerIterator->dx1 = in_out_polygon->v3.x - in_out_polygon->v2.x;
	out_polygonRasterizerIterator->dy1 = in_out_polygon->v3.y - in_out_polygon->v2.y;
	out_polygonRasterizerIterator->e1 = ((out_polygonRasterizerIterator->CurrentPoint.x - in_out_polygon->v3.x) * out_polygonRasterizerIterator->dy1) - ((out_polygonRasterizerIterator->CurrentPoint.y - in_out_polygon->v3.y) * out_polygonRasterizerIterator->dx1);

	//Line 3
	out_polygonRasterizerIterator->dx2 = in_out_polygon->v1.x - in_out_polygon->v3.x;
	out_polygonRasterizerIterator->dy2 = in_out_polygon->v1.y - in_out_polygon->v3.y;
	out_polygonRasterizerIterator->e2 = ((out_polygonRasterizerIterator->CurrentPoint.x - in_out_polygon->v1.x) * out_polygonRasterizerIterator->dy2) - ((out_polygonRasterizerIterator->CurrentPoint.y - in_out_polygon->v1.y) * out_polygonRasterizerIterator->dx2);


	{
		Vector3f l_begin = { (float)in_out_polygon->v2.x - in_out_polygon->v1.x, (float)in_out_polygon->v2.y - in_out_polygon->v1.y, 0.0f };
		Vector3f l_end = { (float)in_out_polygon->v3.x - in_out_polygon->v1.x, (float)in_out_polygon->v3.y - in_out_polygon->v1.y, 0.0f };
		Vector3f l_crossResult;
		Vec_Cross_3f(&l_begin, &l_end, &l_crossResult);

		// Anti-clockwise
		if (l_crossResult.z > 0.0f)
		{
			out_polygonRasterizerIterator->dx0 *= -1; out_polygonRasterizerIterator->dy0 *= -1;
			out_polygonRasterizerIterator->e0 *= -1;
			out_polygonRasterizerIterator->dx1 *= -1; out_polygonRasterizerIterator->dy1 *= -1;
			out_polygonRasterizerIterator->e1 *= -1;
			out_polygonRasterizerIterator->dx2 *= -1; out_polygonRasterizerIterator->dy2 *= -1;
			out_polygonRasterizerIterator->e2 *= -1;
		}
	}

	out_polygonRasterizerIterator->LineIndexCursor = out_polygonRasterizerIterator->PolygonBoundClip.Min.y;
	out_polygonRasterizerIterator->ColumnIndexCursor = out_polygonRasterizerIterator->PolygonBoundClip.Min.x - 1;
	out_polygonRasterizerIterator->CurrentStep = POLYGONRASTERIZER_ITERATOR_STEP_NEWCOLUMN;
}

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
		}

		p_polygonRasterizerIterator->ColumnIndexCursor = p_polygonRasterizerIterator->PolygonBoundClip.Min.x - 1;
		p_polygonRasterizerIterator->CurrentPoint.x = p_polygonRasterizerIterator->PolygonBoundClip.Min.x;

		p_polygonRasterizerIterator->e0 -= ((p_polygonRasterizerIterator->PolygonBoundClip.Max.x - p_polygonRasterizerIterator->PolygonBoundClip.Min.x) * p_polygonRasterizerIterator->dy0);
		p_polygonRasterizerIterator->e1 -= ((p_polygonRasterizerIterator->PolygonBoundClip.Max.x - p_polygonRasterizerIterator->PolygonBoundClip.Min.x) * p_polygonRasterizerIterator->dy1);
		p_polygonRasterizerIterator->e2 -= ((p_polygonRasterizerIterator->PolygonBoundClip.Max.x - p_polygonRasterizerIterator->PolygonBoundClip.Min.x) * p_polygonRasterizerIterator->dy2);

		p_polygonRasterizerIterator->CurrentPoint.y = p_polygonRasterizerIterator->LineIndexCursor;

		p_polygonRasterizerIterator->e0 -= p_polygonRasterizerIterator->dx0;
		p_polygonRasterizerIterator->e1 -= p_polygonRasterizerIterator->dx1;
		p_polygonRasterizerIterator->e2 -= p_polygonRasterizerIterator->dx2;

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
		}

		p_polygonRasterizerIterator->CurrentPoint.x = p_polygonRasterizerIterator->ColumnIndexCursor + 1;

		p_polygonRasterizerIterator->e0 += p_polygonRasterizerIterator->dy0;
		p_polygonRasterizerIterator->e1 += p_polygonRasterizerIterator->dy1;
		p_polygonRasterizerIterator->e2 += p_polygonRasterizerIterator->dy2;

		return l_returnCode;
	}
	break;
	}

	return POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END;
};

#if 0
void Rasterize_PolygonClipped(const Polygon2i_PTR in_out_polygon, Array_Vector2i_PTR out_rasterizedPixels, const Recti_PTR p_clip_rect)
{
	//Calculate polygon rect bound clipped
	Recti l_polygonBoundClip;
	Polygon_BoundingRect_2i(in_out_polygon, &l_polygonBoundClip);
	Rect_ClipPoint_Int(p_clip_rect, &l_polygonBoundClip.Min);
	Rect_ClipPoint_Int(p_clip_rect, &l_polygonBoundClip.Max);

	Vector2i l_point = l_polygonBoundClip.Min;

	//Line 1
	int l_dX0 = in_out_polygon->v2.x - in_out_polygon->v1.x; int l_dY0 = in_out_polygon->v2.y - in_out_polygon->v1.y;
	int l_e0 = ((l_point.x - in_out_polygon->v2.x) * l_dY0) - ((l_point.y - in_out_polygon->v2.y) * l_dX0);

	//Line 2
	int l_dX1 = in_out_polygon->v3.x - in_out_polygon->v2.x; int l_dY1 = in_out_polygon->v3.y - in_out_polygon->v2.y;
	int l_e1 = ((l_point.x - in_out_polygon->v3.x) * l_dY1) - ((l_point.y - in_out_polygon->v3.y) * l_dX1);

	//Line 3
	int l_dX2 = in_out_polygon->v1.x - in_out_polygon->v3.x; int l_dY2 = in_out_polygon->v1.y - in_out_polygon->v3.y;
	int l_e2 = ((l_point.x - in_out_polygon->v1.x) * l_dY2) - ((l_point.y - in_out_polygon->v1.y) * l_dX2);


	{
		Vector3f l_begin = { (float)in_out_polygon->v2.x - in_out_polygon->v1.x, (float)in_out_polygon->v2.y - in_out_polygon->v1.y, 0.0f };
		Vector3f l_end = { (float)in_out_polygon->v3.x - in_out_polygon->v1.x, (float)in_out_polygon->v3.y - in_out_polygon->v1.y, 0.0f };
		Vector3f l_crossResult;
		Vec_Cross_3f(&l_begin, &l_end, &l_crossResult);

		// Anti-clockwise
		if (l_crossResult.z > 0.0f)
		{
			l_dX0 *= -1; l_dY0 *= -1;
			l_e0 *= -1;
			l_dX1 *= -1; l_dY1 *= -1;
			l_e1 *= -1;
			l_dX2 *= -1; l_dY2 *= -1;
			l_e2 *= -1;
		}
	}

	//TODO -> Highly unoptimized, we iterate over the whole polygon bound clip.
	// We can easily implement a smart iteration that try to navigates inside the polygon and try to find edges (thus, ignoring calcuation of empty )
	for (int y = l_polygonBoundClip.Min.y; y <= l_polygonBoundClip.Max.y; y++)
	{
		for (int x = l_polygonBoundClip.Min.x; x < l_polygonBoundClip.Max.x; x++)
		{
			if (l_e0 >= 0 && l_e1 >= 0 && l_e2 >= 0)
			{
				out_rasterizedPixels->Memory[out_rasterizedPixels->Size] = l_point;
				out_rasterizedPixels->Size += 1;
			}

			l_point.x = x + 1;

			l_e0 += l_dY0;
			l_e1 += l_dY1;
			l_e2 += l_dY2;
		}

		if (l_e0 >= 0 && l_e1 >= 0 && l_e2 >= 0)
		{
			out_rasterizedPixels->Memory[out_rasterizedPixels->Size] = l_point;
			out_rasterizedPixels->Size += 1;
		}

		l_point.x = l_polygonBoundClip.Min.x;

		l_e0 -= ((l_polygonBoundClip.Max.x - l_polygonBoundClip.Min.x) * l_dY0);
		l_e1 -= ((l_polygonBoundClip.Max.x - l_polygonBoundClip.Min.x) * l_dY1);
		l_e2 -= ((l_polygonBoundClip.Max.x - l_polygonBoundClip.Min.x) * l_dY2);

		l_point.y = y + 1;

		l_e0 -= l_dX0;
		l_e1 -= l_dX1;
		l_e2 -= l_dX2;
	}

#ifndef NDEBUG
	if (out_rasterizedPixels->Size > out_rasterizedPixels->Capacity)
	{
		printf("Rasterizer::Rasterize_PolygonClipped : not enough memory allocated to the Rasterized pixel buffer");
		abort();
	}
#endif
	};

#endif
