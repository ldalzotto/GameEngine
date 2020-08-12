#pragma once

#include <stdint.h>

#include "v2/Vector/RVector.h"
#include "DataStructures/GenericArray.hpp"

namespace _RenderV2
{
	struct RTexture
	{
		char* Texture;
		uint16_t Width, Height;
		size_t ElementSize;
	};

	struct RTexturePixelCoordinates
	{
		int X, Y;
	};

	void RTexture_fill(const RTexture* p_texture, const void* p_color);
	void RTexture_drawPixels(const RTexture* p_texture, const RTexturePixelCoordinates* p_pixelCoordinates, const void* p_colors, const size_t p_pixelNumbers);
	void RTexture_drawPixel(const RTexture* p_texture, const RTexturePixelCoordinates* p_pixelCoordinate, const void* p_color);
}