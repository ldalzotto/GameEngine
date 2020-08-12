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
	void Rasterizer::line_v2(const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, _Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels)
	{
		_MathV2::Vector<2, int> l_begin_int = { nearbyintf(p_begin->x), nearbyintf(p_begin->y) };
		_MathV2::Vector<2, int> l_end_int = { nearbyintf(p_end->x), nearbyintf(p_end->y) };

		int	l_dx = abs(l_end_int.x - l_begin_int.x); int l_sx = l_begin_int.x < l_end_int.x ? 1 : -1;
		int	l_dy = -abs(l_end_int.y - l_begin_int.y); int l_sy = l_begin_int.y < l_end_int.y ? 1 : -1;
		int err = l_dx + l_dy;
		int e2;

		for (;;)
		{
			_MathV2::Vector<2, int> l_pixel{ l_begin_int.x, l_begin_int.y };
			memcpy(((char*)out_rasterizedPixels->Memory) + (out_rasterizedPixels->ElementSize * out_rasterizedPixels->Size), &l_pixel, sizeof(l_pixel));
			out_rasterizedPixels->Size += 1;
			// _Core::VectorT_pushBack(out_rasterizedPixels, &l_pixel);
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

	void Rasterizer::line_interpolation(const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end,
		const _MathV2::Vector<4, float>* p_begin_cameraSpace, const _MathV2::Vector<4, float>* p_end_cameraSpace, const _Core::ArraySliceT<_MathV2::Vector<2, int>>* p_pixels,
		_Core::VectorT < LineRasterizationInterpolation >* in_out_interpolationResult)
	{
		_MathV2::Vector<2, int> l_begin_int = { nearbyintf(p_begin->x), nearbyintf(p_begin->y) };
		_MathV2::Vector<2, int> l_end_int = { nearbyintf(p_end->x), nearbyintf(p_end->y) };

		float l_screenDistance_line = _MathV2::VectorM::distance(&_MathV2::VectorM::cast_if(&l_begin_int), &_MathV2::VectorM::cast_if(&l_end_int));

		_Core::VectorRangedIteratorT<_MathV2::Vector<2, int>> l_pixels_it = _Core::ArraySliceT_buildIterator(p_pixels);
		while (_Core::VectorRangedIteratorT_moveNext(&l_pixels_it))
		{
			LineRasterizationInterpolation* l_interpolation = _Core::VectorT_at(in_out_interpolationResult, l_pixels_it.CurrentIndex);
			_MathV2::Vector2<int>* l_pixelCoord = l_pixels_it.Current;

			float l_s = _MathV2::VectorM::distance(&_MathV2::VectorM::cast_if(&l_end_int), &_MathV2::VectorM::cast_if(l_pixelCoord)) / l_screenDistance_line;
			float l_z_factor = 1.0f / ((1.0f / p_begin_cameraSpace->z) + l_s * ((1.0f / p_end_cameraSpace->z) - (1.0f / p_begin_cameraSpace->z)));
			float l_t = (l_z_factor - p_begin_cameraSpace->z) / (p_end_cameraSpace->z - p_begin_cameraSpace->z);

			// out_interpolationResult->
			l_interpolation->L1 = l_t;
			l_interpolation->L2 = 1 - l_t;
		}
	};

	/*
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
	*/
}