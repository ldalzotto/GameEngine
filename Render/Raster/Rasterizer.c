#include "Rasterizer.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "Constants.h"
#include "v2/_interface/VectorStructuresC.h"
#include "v2/_interface/RectC.h"
#include "v2/_interface/VectorC.h"
#include "Error/ErrorHandler.h"


//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
// A possible performance improvement is to store pixel steps in out_rasterizedPixels instead of plain pixel coordinates.
// The the consumer can create a cursor to final texture memory and move it to set appropriate color.
char Rasterrize_Line(Vector2i_PTR p_begin, Vector2i_PTR p_end, ARRAY_RASTERISATIONSTEP_PTR out_rasterizedPixels)
{
	int	l_dx = abs(p_end->x - p_begin->x); int l_sx = p_begin->x < p_end->x ? 1 : -1;
	int	l_dy = -abs(p_end->y - p_begin->y); int l_sy = p_begin->y < p_end->y ? 1 : -1;
	int err = l_dx + l_dy;
	int e2;

	RasterizationStepDirection l_xStep = l_sx > 0 ? RasterizationStepDirection_ADD : RasterizationStepDirection_REMOVE;
	RasterizationStepDirection l_yStep = l_sy > 0 ? RasterizationStepDirection_ADD : RasterizationStepDirection_REMOVE;

	size_t l_rasterizedPixels_bufferIndex = 0;
	RASTERIZATIONSTEP l_rasterisationStep = { 0 };

	for (;;)
	{
		out_rasterizedPixels->Memory[l_rasterizedPixels_bufferIndex] = l_rasterisationStep;
		l_rasterizedPixels_bufferIndex += 1;
		l_rasterisationStep = (RASTERIZATIONSTEP){ 0 };

		e2 = 2 * err;
		if (e2 >= l_dy)
		{
			if (p_begin->x == p_end->x)
			{
				break;
			}
			err += l_dy;
			p_begin->x += l_sx;
			l_rasterisationStep.XDirection = l_xStep;
		}
		if (e2 <= l_dx)
		{
			if (p_begin->y == p_end->y)
			{
				break;
			}
			err += l_dx;
			p_begin->y += l_sy;
			l_rasterisationStep.YDirection = l_yStep;
		}
	}

	out_rasterizedPixels->Size = l_rasterizedPixels_bufferIndex - 1;

#ifndef NDEBUG
	if (out_rasterizedPixels->Size > out_rasterizedPixels->Capacity)
	{
		printf("Rasterizer::line_v3 : not enough memory allocated to the Rasterized pixel buffer");
		return 1;
	}
#endif

	return 0;
};

bool Rasterize_LineClipped(
	const Vector2i_PTR p_begin, const Vector2i_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR out_rasterizedPixels,
	const Recti_PTR p_clip_rect,
	Vector2i_PTR out_clipped_begin,
	Vector2i_PTR out_clipped_end)
{
	if (Rect_ClipSegment_Int(p_begin, p_end, p_clip_rect, out_clipped_begin, out_clipped_end))
	{
		HANDLE_ERR(Rasterrize_Line(out_clipped_begin, out_clipped_end, out_rasterizedPixels));
		return true;
	};
	return false;
}

//A Parallel Algorithm for Polygon Rasterization.pdf
void Rasterize_PolygonClipped(const Polygon2i_PTR p_polygon, Array_Vector2i_PTR out_rasterizedPixels, const Recti_PTR p_clip_rect)
{
	// Arr_Clear(&out_rasterizedPixels->array);

	//Calculate polygon rect bound clipped
	Recti l_polygonBoundClip = {
		.Min = p_polygon->v1,
		.Max = p_polygon->v1
	};
	for (short int i = 1; i < 3; i++)
	{
		if (p_polygon->Points[i].x < l_polygonBoundClip.Min.x) { l_polygonBoundClip.Min.x = p_polygon->Points[i].x; }
		if (p_polygon->Points[i].x > l_polygonBoundClip.Max.x) { l_polygonBoundClip.Max.x = p_polygon->Points[i].x; }
		if (p_polygon->Points[i].y < l_polygonBoundClip.Min.y) { l_polygonBoundClip.Min.y = p_polygon->Points[i].y; }
		if (p_polygon->Points[i].y > l_polygonBoundClip.Max.y) { l_polygonBoundClip.Max.y = p_polygon->Points[i].y; }
	}
	Rect_ClipPoint_Int(p_clip_rect, &l_polygonBoundClip.Min);
	Rect_ClipPoint_Int(p_clip_rect, &l_polygonBoundClip.Max);

	// Vector2i l_begin = { p_polygon->Points[1].x - p_polygon->Points[0].x, p_polygon->Points[1].y - p_polygon->Points[0].y };
	// Vector2i l_end = { p_polygon->Points[2].x - p_polygon->Points[0].x, p_polygon->Points[2].y - p_polygon->Points[0].y };
	 // double l_angle = Vec_SignedAngle_2i(&l_begin, &l_end);

	Vector2i l_point = l_polygonBoundClip.Min;

	//Line 1
	int l_dX0 = p_polygon->v2.x - p_polygon->v1.x; int l_dY0 = p_polygon->v2.y - p_polygon->v1.y;
	int l_e0 = ((l_point.x - p_polygon->v2.x) * l_dY0) - ((l_point.y - p_polygon->v2.y) * l_dX0);

	//Line 2
	int l_dX1 = p_polygon->v3.x - p_polygon->v2.x; int l_dY1 = p_polygon->v3.y - p_polygon->v2.y;
	int l_e1 = ((l_point.x - p_polygon->v3.x) * l_dY1) - ((l_point.y - p_polygon->v3.y) * l_dX1);

	//Line 3
	int l_dX2 = p_polygon->v1.x - p_polygon->v3.x; int l_dY2 = p_polygon->v1.y - p_polygon->v3.y;
	int l_e2 = ((l_point.x - p_polygon->v1.x) * l_dY2) - ((l_point.y - p_polygon->v1.y) * l_dX2);

	for (int y = l_polygonBoundClip.Min.y; y <= l_polygonBoundClip.Max.y; y++)
	{
		for (int x = l_polygonBoundClip.Min.x; x < l_polygonBoundClip.Max.x; x++)
		{
			if (l_e0 >= 0 && l_e1 >= 0 && l_e2 >= 0)
			{
				Arr_PushBackRealloc_Vector2i(out_rasterizedPixels, &l_point);
			}

			l_point.x = x + 1;

			l_e0 += l_dY0;
			l_e1 += l_dY1;
			l_e2 += l_dY2;
		}

		if (l_e0 >= 0 && l_e1 >= 0 && l_e2 >= 0)
		{
			Arr_PushBackRealloc_Vector2i(out_rasterizedPixels, &l_point);
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
};