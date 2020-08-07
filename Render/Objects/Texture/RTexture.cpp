#include "RTexture.hpp"

#include <cstdlib>
#include <cstring>

namespace _RenderV2
{
	void RTexture_fill(char* p_texture, const void* p_color, const size_t p_elementSize, const size_t p_pixelCount)
	{
		for (size_t i = 0; i < p_pixelCount; i++)
		{
			char* l_pixel = p_texture + (i * p_elementSize);
			memcpy(l_pixel, p_color, p_elementSize);
		}
	};

}