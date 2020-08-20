#include "DrawFunctions.h"

#include "Raster/Rasterizer.h"

#include "Objects/Texture/Texture.h"

void Draw_LineClipped(
	VECTOR2F_PTR p_begin, VECTOR2F_PTR p_end,
	ARRAY_RASTERISATIONSTEP_PTR in_out_rasterizedPixelsBuffer,
	TEXTURE3C_PTR p_to, RECTI_PTR p_clipRect,
	VECTOR3C_PTR p_color)
{
	Arr_Clear_RasterisationStep(in_out_rasterizedPixelsBuffer);

	VECTOR2I l_clippedBegin, l_clippedEnd;
	if (Rasterize_LineClipped(p_begin, p_end, in_out_rasterizedPixelsBuffer, p_clipRect, &l_clippedBegin, &l_clippedEnd))
	{

		char* l_to_memory_cursor = (char*)p_to->Pixels.Memory + Texture_GetElementOffset_3C(l_clippedBegin.x, l_clippedBegin.y, p_to->Width);
		for (size_t j = 0; j < in_out_rasterizedPixelsBuffer->Size; j++)
		{
			if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection_ADD)
			{
				l_to_memory_cursor -= sizeof(VECTOR3C);
			}
			else if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection_REMOVE)
			{
				l_to_memory_cursor += sizeof(VECTOR3C);
			}

			if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection_ADD)
			{
				l_to_memory_cursor -= (sizeof(VECTOR3C) * p_to->Width);
			}
			else if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection_REMOVE)
			{
				l_to_memory_cursor += (sizeof(VECTOR3C) * p_to->Width);
			}
			*(VECTOR3C_PTR)l_to_memory_cursor = *p_color;
		}
	}
};
