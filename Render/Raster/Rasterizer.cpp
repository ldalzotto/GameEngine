#include "Rasterizer.hpp"

#include <math.h>
#include <stdexcept>

#include "Constants.h"
#include "v2/Vector/VectorMath.hpp"
extern "C"
{
#include "v2/_interface/RectC.h"
}
#include "DataStructures/Specifications/VectorT.hpp"
#include <string.h>

namespace _RenderV2
{

	//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	// A possible performance improvement is to store pixel steps in out_rasterizedPixels instead of plain pixel coordinates.
	// The the consumer can create a cursor to final texture memory and move it to set appropriate color.
	void line_v3(_MathV2::Vector<2, int>* p_begin, _MathV2::Vector<2, int>* p_end, _Core::VectorT<RasterizationStep>* out_rasterizedPixels)
	{
		int	l_dx = abs(p_end->x - p_begin->x); int l_sx = p_begin->x < p_end->x ? 1 : -1;
		int	l_dy = -abs(p_end->y - p_begin->y); int l_sy = p_begin->y < p_end->y ? 1 : -1;
		int err = l_dx + l_dy;
		int e2;

		RasterizationStepDirection l_xStep = l_sx > 0 ? RasterizationStepDirection::ADD : RasterizationStepDirection::REMOVE;
		RasterizationStepDirection l_yStep = l_sy > 0 ? RasterizationStepDirection::ADD : RasterizationStepDirection::REMOVE;

		size_t l_rasterizedPixels_bufferIndex = 0;
		RasterizationStep l_rasterisationStep{};

		for (;;)
		{
			out_rasterizedPixels->Memory[l_rasterizedPixels_bufferIndex] = l_rasterisationStep;
			l_rasterizedPixels_bufferIndex += 1;
			l_rasterisationStep = {};

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
			throw std::exception("Rasterizer::line_v3 : not enough memory allocated to the Rasterized pixel buffer");
		}
#endif
	};

	bool Rasterizer::line_v3_clipped(
		const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end,
		_Core::VectorT<RasterizationStep>* out_rasterizedPixels,
		const RECTI_PTR p_clip_rect,
		_MathV2::Vector<2, int>* out_clipped_begin,
		_MathV2::Vector<2, int>* out_clipped_end
	)
	{
		if (Rect_ClipSegment_Int((VECTOR2F_PTR)p_begin, (VECTOR2F_PTR)p_end, p_clip_rect, (VECTOR2I_PTR)out_clipped_begin, (VECTOR2I_PTR)out_clipped_end))
		{
			line_v3(out_clipped_begin, out_clipped_end, out_rasterizedPixels);
			return true;
		};
		return false;

		/*
			_MathV2::Vector<2, int> l_beginClipped = *p_begin;
	_MathV2::Vector<2, int> l_endClipped = *p_end;

		line_v3(&l_beginClipped, &l_endClipped, out_rasterizedPixels);

		size_t l_initialSize = out_rasterizedPixels->Size;
		for (size_t i = out_rasterizedPixels->Size; i--; i <= l_initialSize)
		{
			if (out_rasterizedPixels->Memory[i].x < 0 || out_rasterizedPixels->Memory[i].x >= p_clip_rect->Max.x ||
				out_rasterizedPixels->Memory[i].y < 0 || out_rasterizedPixels->Memory[i].y >= p_clip_rect->Max.y)
			{
				_Core::VectorT_erase(out_rasterizedPixels, i);
			}
		}
		*/
	}

}