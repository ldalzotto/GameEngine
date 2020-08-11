#include "Rasterizer.hpp"

#include <math.h>
#include "v2/Vector/VectorMath.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

namespace _RenderV2
{
	//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	void Rasterizer::line(
		const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end,
		const _MathV2::Vector<4, float>* p_begin_cameraSpace, const _MathV2::Vector<4, float>* p_end_cameraSpace,
		bool l_interpolate, _Core::VectorT<LineRasterizationResult>* out_rasterizedPixels
	)
	{
		size_t l_rasterizedPixels_beginSize = out_rasterizedPixels->Size;
		{
			_MathV2::Vector<2, int> l_begin_int = { nearbyintf(p_begin->x), nearbyintf(p_begin->y) };
			_MathV2::Vector<2, int> l_end_int = { nearbyintf(p_end->x), nearbyintf(p_end->y) };

			int	l_dx = abs(l_end_int.x - l_begin_int.x); int l_sx = l_begin_int.x < l_end_int.x ? 1 : -1;
			int	l_dy = -abs(l_end_int.y - l_begin_int.y); int l_sy = l_begin_int.y < l_end_int.y ? 1 : -1;
			int err = l_dx + l_dy;
			int e2;

			for (;;)
			{
				LineRasterizationResult l_pixel{};
				l_pixel.PixelCoord = { l_begin_int.x, l_begin_int.y };
				_Core::VectorT_pushBack(out_rasterizedPixels, &l_pixel);
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
		}

		size_t l_rasterizedPixels_endSize = out_rasterizedPixels->Size;

		{
			if (l_interpolate)
			{
				_MathV2::Vector<2, int> l_begin_int = { nearbyintf(p_begin->x), nearbyintf(p_begin->y) };
				_MathV2::Vector<2, int> l_end_int = { nearbyintf(p_end->x), nearbyintf(p_end->y) };

				float l_screenDistance_line = _MathV2::VectorM::distance(&_MathV2::VectorM::cast_if(&l_begin_int), &_MathV2::VectorM::cast_if(&l_end_int));

				for (size_t i = l_rasterizedPixels_beginSize; i < l_rasterizedPixels_endSize; i++)
				{
					LineRasterizationResult* l_rasterizationResult = _Core::VectorT_at(out_rasterizedPixels, i);

					float l_s = _MathV2::VectorM::distance(&_MathV2::VectorM::cast_if(&l_end_int), &_MathV2::VectorM::cast_if(&l_rasterizationResult->PixelCoord)) / l_screenDistance_line;
					float l_z_factor = 1.0f / ((1.0f / p_begin_cameraSpace->z) + l_s * ((1.0f / p_end_cameraSpace->z) - (1.0f / p_begin_cameraSpace->z)));
					float l_t = (l_z_factor - p_begin_cameraSpace->z) / (p_end_cameraSpace->z - p_begin_cameraSpace->z);

					l_rasterizationResult->L1 = l_t;
					l_rasterizationResult->L2 = 1 - l_t;
				}
			}
		}
		
	};
}