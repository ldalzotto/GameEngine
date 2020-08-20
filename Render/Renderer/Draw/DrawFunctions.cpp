#include "DrawFunctions.hpp"

#include "DataStructures/Specifications/VectorT.hpp"
#include "Raster/Rasterizer.hpp"

extern "C"
{
#include "Objects/Texture/Texture.h"
}

namespace _RenderV2
{
	void DrawM::DrawLineClipped(
		VECTOR2F_PTR p_begin, VECTOR2F_PTR p_end,
		_Core::VectorT<RasterizationStep>* in_out_rasterizedPixelsBuffer,
		TEXTURE3C_PTR p_to, RECTI_PTR p_clipRect,
		VECTOR3C_PTR p_color)
	{
		_Core::VectorT_clear(in_out_rasterizedPixelsBuffer);

		VECTOR2I l_clippedBegin, l_clippedEnd;
		if (Rasterizer::line_v3_clipped(p_begin, p_end, in_out_rasterizedPixelsBuffer, p_clipRect, &l_clippedBegin, &l_clippedEnd))
		{

			char* l_to_memory_cursor = (char*)p_to->Pixels.Memory + Texture_GetElementOffset_3C(l_clippedBegin.x, l_clippedBegin.y, p_to->Width);
			for (size_t j = 0; j < in_out_rasterizedPixelsBuffer->Size; j++)
			{
				if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection::ADD)
				{
					l_to_memory_cursor -= sizeof(VECTOR3C);
				}
				else if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection::REMOVE)
				{
					l_to_memory_cursor += sizeof(VECTOR3C);
				}

				if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection::ADD)
				{
					l_to_memory_cursor -= (sizeof(VECTOR3C) * p_to->Width);
				}
				else if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection::REMOVE)
				{
					l_to_memory_cursor += (sizeof(VECTOR3C) * p_to->Width);
				}
				*(VECTOR3C_PTR)	l_to_memory_cursor = *p_color;
			}
		}
	};
}