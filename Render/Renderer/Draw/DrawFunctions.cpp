#include "DrawFunctions.hpp"

#include "DataStructures/Specifications/VectorT.hpp"
#include "Raster/Rasterizer.hpp"
#include "Objects/Texture/TextureM.hpp"

namespace _RenderV2
{
	void DrawM::DrawLineClipped(
		_MathV2::Vector<2, float>* p_begin, _MathV2::Vector<2, float>* p_end,
		_Core::VectorT<RasterizationStep>* in_out_rasterizedPixelsBuffer,
		Texture<3, char>* p_to, RECTI_PTR  p_clipRect,
		_MathV2::Vector<3, char>* p_color)
	{
		_Core::VectorT_clear(in_out_rasterizedPixelsBuffer);

		_MathV2::Vector<2, int> l_clippedBegin, l_clippedEnd;
		if (Rasterizer::line_v3_clipped(p_begin, p_end, in_out_rasterizedPixelsBuffer, p_clipRect, &l_clippedBegin, &l_clippedEnd))
		{

			char* l_to_memory_cursor = (char*)p_to->Pixels.Memory + TextureM::getElementOffset(l_clippedBegin.x, l_clippedBegin.y, p_to->Width, p_to->Pixels.ElementSize);
			for (size_t j = 0; j < in_out_rasterizedPixelsBuffer->Size; j++)
			{
				if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection::ADD)
				{
					l_to_memory_cursor -= p_to->Pixels.ElementSize;
				}
				else if (in_out_rasterizedPixelsBuffer->Memory[j].XDirection == RasterizationStepDirection::REMOVE)
				{
					l_to_memory_cursor += p_to->Pixels.ElementSize;
				}

				if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection::ADD)
				{
					l_to_memory_cursor -= (p_to->Pixels.ElementSize * p_to->Width);
				}
				else if (in_out_rasterizedPixelsBuffer->Memory[j].YDirection == RasterizationStepDirection::REMOVE)
				{
					l_to_memory_cursor += (p_to->Pixels.ElementSize * p_to->Width);
				}
				*(_MathV2::Vector3<char>*)	l_to_memory_cursor = *p_color;
			}
		}
	};
}