#pragma once

#include <stdint.h>

#include "DataStructures/Specifications/ArrayT.hpp"
extern "C"
{
#include "v2/_interface/VectorC_def.h"
}

namespace _RenderV2
{
	struct Texture3C
	{
		uint16_t Width, Height;
		_Core::ArrayT<VECTOR3C> Pixels;
	};

	struct TextureIterator3C
	{
		Texture3C* Texture;
		VECTOR3C_PTR Current;
		size_t Index;
	};
}