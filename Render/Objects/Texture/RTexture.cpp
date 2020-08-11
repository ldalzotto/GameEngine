#include "RTexture.hpp"

#include <cstdlib>
#include <cstring>

namespace _RenderV2
{

	inline char* RTexture_getPixel(const RTexture* p_texture, int p_x, int p_y)
	{
		return p_texture->Texture + ((((size_t)p_y * p_texture->Width) + p_x) * p_texture->ElementSize);
	};

	void RTexture_fill(char* p_texture, const void* p_color, const size_t p_elementSize, const size_t p_pixelCount)
	{
		for (size_t i = 0; i < p_pixelCount; i++)
		{
			char* l_pixel = p_texture + (i * p_elementSize);
			memcpy(l_pixel, p_color, p_elementSize);
		}
	};

	void RTexture_drawPixels(const RTexture* p_texture, const RTexturePixelCoordinates* p_pixelCoordinates, const void* p_colors, const size_t p_pixelNumbers)
	{
		for (size_t i = 0; i < p_pixelNumbers; i++)
		{
			RTexturePixelCoordinates* l_pixelCoord = (RTexturePixelCoordinates*)(((char*)p_pixelCoordinates) + (i * sizeof(RTexturePixelCoordinates)));
			char* l_colorMemory = (char*)p_colors + (i * p_texture->ElementSize);
			memcpy(RTexture_getPixel(p_texture, l_pixelCoord->X, l_pixelCoord->Y), l_colorMemory, p_texture->ElementSize);
		}
	};

	void RTexture_drawPixel(const RTexture* p_texture, const RTexturePixelCoordinates* p_pixelCoordinate, const void* p_color)
	{
		memcpy(RTexture_getPixel(p_texture, p_pixelCoordinate->X, p_pixelCoordinate->Y), p_color, p_texture->ElementSize);
	};
}