
#include "ClipMath.hpp"
#include "Constants.hpp"

namespace _MathV2
{
	enum class ClipPosition : short int
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

	ClipPosition calculateClip(const _MathV2::Vector<2, int>* p_point, const _MathV2::Rect<int>* p_clipRect)
	{
		short int l_pos = (short int)ClipPosition::INSIDE;

		if (p_point->y > p_clipRect->Max.y)
		{
			l_pos |= (short int)ClipPosition::UP;
		}
		else if (p_point->y < p_clipRect->Min.y)
		{
			l_pos |= (short int)ClipPosition::DOWN;
		}

		if (p_point->x > p_clipRect->Max.x)
		{
			l_pos |= (short int)ClipPosition::RIGHT;
		}
		else if (p_point->x < p_clipRect->Min.x)
		{
			l_pos |= (short int)ClipPosition::LEFT;
		}

		return (ClipPosition)l_pos;
	}


	bool ClipM::clip(const Vector<2, int>* in_clippedSegment_begin, const Vector<2, int>* in_clippedSegment_end,
		const Rect<int>* p_clippedRect, Vector<2, int>* out_clippedSegment_begin, Vector<2, int>* out_clippedSegment_end)
	{
	
		ClipPosition l_beginClip = calculateClip(in_clippedSegment_begin, p_clippedRect);
		ClipPosition l_endClip = calculateClip(in_clippedSegment_end, p_clippedRect);

		*out_clippedSegment_begin = *in_clippedSegment_begin;
		*out_clippedSegment_end = *in_clippedSegment_end;

		if ((l_beginClip != ClipPosition::INSIDE) || (l_endClip != ClipPosition::INSIDE))
		{
			ClipPosition l_beginClippedClip = l_beginClip;
			ClipPosition l_endClippedClip = l_endClip;

			// Not the same region
			while (true)
			{
				if ((l_beginClippedClip == ClipPosition::INSIDE) && (l_endClippedClip == ClipPosition::INSIDE))
				{
					break;
				}
				else if ((short int)l_beginClippedClip & (short int)l_endClippedClip)
				{
					return false;
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

					if ((short int)l_selectedClip & (short int)ClipPosition::UP)
					{
						l_y = p_clippedRect->Max.y;
						// deltaX = slope * (fixed position diff)
						l_x = out_clippedSegment_begin->x + (((out_clippedSegment_end->x - out_clippedSegment_begin->x) / (out_clippedSegment_end->y - out_clippedSegment_begin->y)) * (l_y - out_clippedSegment_begin->y));
					}
					else if ((short int)l_selectedClip & (short int)ClipPosition::DOWN)
					{
						l_y = p_clippedRect->Min.y;
						l_x = out_clippedSegment_begin->x + (((out_clippedSegment_end->x - out_clippedSegment_begin->x) / (out_clippedSegment_end->y - out_clippedSegment_begin->y)) * (l_y - out_clippedSegment_begin->y));
					}
					else if ((short int)l_selectedClip & (short int)ClipPosition::RIGHT)
					{
						l_x = p_clippedRect->Max.x;
						l_y = out_clippedSegment_begin->y + (((out_clippedSegment_end->y - out_clippedSegment_begin->y) / (out_clippedSegment_end->x - out_clippedSegment_begin->x)) * (l_x - out_clippedSegment_begin->x));
					}
					else if ((short int)l_selectedClip & (short int)ClipPosition::LEFT)
					{
						l_x = p_clippedRect->Min.x;
						l_y = out_clippedSegment_begin->y + (((out_clippedSegment_end->y - out_clippedSegment_begin->y) / (out_clippedSegment_end->x - out_clippedSegment_begin->x)) * (l_x - out_clippedSegment_begin->x));
					}

					if (l_selectedPoint == 0)
					{
						out_clippedSegment_begin->x = l_x;
						out_clippedSegment_begin->y = l_y;
						l_beginClippedClip = calculateClip(out_clippedSegment_begin, p_clippedRect);
					}
					else
					{
						out_clippedSegment_end->x = l_x;
						out_clippedSegment_end->y = l_y;
						l_endClippedClip = calculateClip(out_clippedSegment_end, p_clippedRect);
					}
				}
			}

		}

		return true;

	}
}