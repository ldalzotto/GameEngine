#pragma once

extern "C"
{
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "Objects/Texture/Texture_def.h"
}

namespace _Core
{
	template<typename T>
	struct VectorT;
}

namespace _RenderV2
{
	struct RasterizationStep;
}

namespace _RenderV2
{
	struct DrawM
	{
		static	void DrawLineClipped(
			VECTOR2F_PTR p_begin, VECTOR2F_PTR p_end,
			_Core::VectorT<RasterizationStep>* in_out_rasterizedPixelsBuffer,
			TEXTURE3C_PTR p_to, RECTI_PTR p_clipRect,
			VECTOR3C_PTR p_color);
	};
}