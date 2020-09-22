#include "DrawFunctions.h"

#include "Raster/Rasterizer.h"

void Draw_LineClipped(
	Vector2i_PTR p_begin, Vector2i_PTR p_end,
	Texture3f_PTR p_to, Recti_PTR p_clipRect,
	Color3f_PTR p_color)
{
	LineRasterizerIterator l_lineRasterizerIterator;
	if (LineRasterize_Initialize(p_begin, p_end, p_clipRect, &l_lineRasterizerIterator))
	{
		while (LineRasterize_MoveNext(&l_lineRasterizerIterator))
		{
			p_to->Pixels.Memory[l_lineRasterizerIterator.CurrentPoint.x + (l_lineRasterizerIterator.CurrentPoint.y * p_to->Width)] = *p_color;
		}
	};
};
