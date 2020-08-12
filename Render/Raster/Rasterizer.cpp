#include "Rasterizer.hpp"

#include <math.h>
#include "v2/Vector/VectorMath.hpp"
#include "DataStructures/Specifications/ArraySliceT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include <string.h>

namespace _RenderV2
{

	//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	//TODO - Before executing the rasterization, we can clip the begin and end pixel values to the screen.
	// This will save us some compute time in case of the line is very huge, because we will rasterise even outside of the screen.
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
	};

}