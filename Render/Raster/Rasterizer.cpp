#include "Rasterizer.hpp"

#include <math.h>
#include "Constants.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "DataStructures/Specifications/ArraySliceT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Line/LineMath.hpp"

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
			_MathV2::Vector<2, int> l_pixel = { l_begin_int.x, l_begin_int.y };
			_Core::VectorT_pushBack(out_rasterizedPixels, &l_pixel);
			// out_rasterizedPixels->Memory[l_rasterizedPixels_bufferIndex] = { l_begin_int.x, l_begin_int.y };
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

	enum ClipPosition : short int
	{
		INSIDE = 0,
		UP = 1,
		DOWN = 2,
		LEFT = 4,
		RIGHT = 8,
		UP_LEFT = UP & LEFT,
		UP_RIGHT = UP & RIGHT,
		DOWN_LEFT = DOWN & LEFT,
		DOWN_RIGHT = DOWN & RIGHT
	};

	ClipPosition calculateClip(const _MathV2::Vector<2, float>* p_point, short int p_width, short int p_height)
	{
		short int l_pos = (short int)ClipPosition::INSIDE;

		if (p_point->y > p_height + FLOAT_TOLERANCE)
		{
			l_pos |= ClipPosition::UP;
		}
		else if (p_point->y < -FLOAT_TOLERANCE)
		{
			l_pos |= ClipPosition::DOWN;
		}

		if (p_point->x > p_width + FLOAT_TOLERANCE)
		{
			l_pos |= RIGHT;
		}
		else if (p_point->x < -FLOAT_TOLERANCE)
		{
			l_pos |= LEFT;
		}

		return (ClipPosition)l_pos;
	}

	void Rasterizer::line_v3_clipped(
		const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end,
		_Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels,
		const int p_texWidth, const int p_texHeight)
	{
		bool l_clipped = false;

		ClipPosition l_beginClip = calculateClip(p_begin, p_texWidth, p_texHeight);
		ClipPosition l_endClip = calculateClip(p_end, p_texWidth, p_texHeight);

		if ((l_beginClip != ClipPosition::INSIDE) || (l_endClip != ClipPosition::INSIDE))
		{
			_MathV2::Vector<2, float> l_beginClipped = *p_begin;
			_MathV2::Vector<2, float> l_endClipped = *p_end;

			ClipPosition l_beginClippedClip = l_beginClip;
			ClipPosition l_endClippedClip = l_endClip;

			// Not the same region
			while (true)
			{
				if ((l_beginClippedClip == ClipPosition::INSIDE) && (l_endClippedClip == ClipPosition::INSIDE))
				{
					break;
				}
				else if (l_beginClippedClip & l_endClippedClip)
				{
					return;
				}
				else
				{
					short int l_selectedPoint = 0;
					ClipPosition l_selectedClip;
					if (l_beginClippedClip != ClipPosition::INSIDE)
					{
						l_selectedPoint = 0;
						l_selectedClip = l_beginClippedClip;
					}
					else if (l_endClippedClip != ClipPosition::INSIDE)
					{
						l_selectedPoint = 1;
						l_selectedClip = l_endClippedClip;
					}

					float l_x, l_y;

					if (l_selectedClip & ClipPosition::UP)
					{
						l_y = p_texHeight;
						l_x = l_beginClipped.x + ((p_texWidth / p_texHeight) * (l_endClipped.y - l_y));
					}
					else if (l_selectedClip & ClipPosition::DOWN)
					{
						l_y = 0;
						l_x = l_beginClipped.x + ((p_texWidth / p_texHeight) * (l_endClipped.y - l_y));
					}
					else if (l_selectedClip & ClipPosition::RIGHT)
					{
						l_x = p_texWidth;
						l_y = l_beginClipped.y + ((p_texHeight / p_texWidth) * (l_endClipped.x - l_x));
					}
					else if (l_selectedClip & ClipPosition::LEFT)
					{
						l_x = 0;
						l_y = l_beginClipped.y + ((p_texHeight / p_texWidth) * (l_endClipped.x - l_x));
					}

					if (l_selectedPoint == 0)
					{
						l_beginClipped.x = l_x;
						l_beginClipped.y = l_y;
						l_beginClippedClip = calculateClip(&l_beginClipped, p_texWidth, p_texHeight);
					}
					else
					{
						l_endClipped.x = l_x;
						l_endClipped.y = l_y;
						l_endClippedClip = calculateClip(&l_endClipped, p_texWidth, p_texHeight);
					}
				}
			}


			Rasterizer::line_v3(&l_beginClipped, &l_endClipped, out_rasterizedPixels);

		}
		else
		{
			Rasterizer::line_v3(p_begin, p_end, out_rasterizedPixels);
		}
	}

}