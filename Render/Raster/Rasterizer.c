#include "Rasterizer.h"

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "Constants.h"
#include "v2/_interface/RectC.h"
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
	RASTERIZATIONSTEP l_rasterisationStep = {0};

	for (;;)
	{
		out_rasterizedPixels->Memory[l_rasterizedPixels_bufferIndex] = l_rasterisationStep;
		l_rasterizedPixels_bufferIndex += 1;
		l_rasterisationStep = (RASTERIZATIONSTEP){0};

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
	const Vector2f_PTR p_begin, const Vector2f_PTR p_end,
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