#include "Rasterizer.hpp"

#include <math.h>
#include "v2/Vector/Vector.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

namespace _RenderV2
{
	//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	void Rasterizer::line(const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, _Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels)
	{
		_MathV2::Vector<2, int> l_begin_int = { nearbyintf(p_begin->x), nearbyintf(p_begin->y) };
		_MathV2::Vector<2, int> l_end_int = { nearbyintf(p_end->x), nearbyintf(p_end->y) };

		int	l_dx = abs(l_end_int.x - l_begin_int.x); int l_sx = l_begin_int.x < l_end_int.x ? 1 : -1;
		int	l_dy = -abs(l_end_int.y - l_begin_int.y); int l_sy = l_begin_int.y < l_end_int.y ? 1 : -1;
		int err = l_dx + l_dy;
		int e2;

		for (;;)
		{
			_Core::VectorT_pushBack(out_rasterizedPixels, _MathV2::Vector<2, int>{l_begin_int.x, l_begin_int.y});
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
	};
}