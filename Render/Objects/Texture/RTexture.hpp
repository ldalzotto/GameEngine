#pragma once

#include <stdint.h>

#include "v2/Vector/RVector.h"
#include "DataStructures/GenericArray.hpp"

namespace _RenderV2
{
	void RTexture_fill(char* p_texture, const void* p_color, const size_t p_elementSize, const size_t p_pixelCount);
}