#include "Rasterizer.hpp"

#include <math.h>
#include <stdexcept>

#include "Constants.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Clip/ClipMath.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Rect/Rect.hpp"
#include <string.h>

namespace _RenderV2
{

	//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	void Rasterizer::line_v3(const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, _Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels)
	{
		_MathV2::Vector<2, int> l_begin_int = { rintf(p_begin->x), rintf(p_begin->y) };
		_MathV2::Vector<2, int> l_end_int = { rintf(p_end->x), rintf(p_end->y) };

		int	l_dx = abs(l_end_int.x - l_begin_int.x); int l_sx = l_begin_int.x < l_end_int.x ? 1 : -1;
		int	l_dy = -abs(l_end_int.y - l_begin_int.y); int l_sy = l_begin_int.y < l_end_int.y ? 1 : -1;
		int err = l_dx + l_dy;
		int e2;

		size_t l_rasterizedPixels_bufferIndex = 0;

		for (;;)
		{
			_MathV2::Vector<2, int> l_pixel = { l_begin_int.x, l_begin_int.y };
			// _Core::VectorT_pushBack(out_rasterizedPixels, &l_pixel);
			out_rasterizedPixels->Memory[l_rasterizedPixels_bufferIndex] = { l_begin_int.x, l_begin_int.y };
			l_rasterizedPixels_bufferIndex += 1;

			e2 = 2 * err;
			if (e2 >= l_dy)
			{
				if (l_begin_int.x == l_end_int.x)
				{
					break;
				}
				err += l_dy;
				l_begin_int.x += l_sx;
			}
			if (e2 <= l_dx)
			{
				if (l_begin_int.y == l_end_int.y)
				{
					break;
				}
				err += l_dx;
				l_begin_int.y += l_sy;
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

	void Rasterizer::line_v3_clipped(
		const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end,
		_Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels,
		const _MathV2::Rect<int>* p_clip_rect)
	{
		_MathV2::Vector<2, float> l_beginClipped;
		_MathV2::Vector<2, float> l_endClipped;
		if (_MathV2::ClipM::clip(p_begin, p_end, p_clip_rect, &l_beginClipped, &l_endClipped))
		{
			Rasterizer::line_v3(&l_beginClipped, &l_endClipped, out_rasterizedPixels);
		};
	}

}