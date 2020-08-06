#pragma once

#include <stdint.h>

#include "DataStructures/Specifications/ArrayT.hpp"

#include "v2/Vector/Vector.hpp"

namespace _RenderV2
{
	template <int N, typename T>
	struct Texture
	{
		uint16_t Width, Height;
		_Core::ArrayT<_MathV2::Vector<N, T>> Pixels;
	};
}