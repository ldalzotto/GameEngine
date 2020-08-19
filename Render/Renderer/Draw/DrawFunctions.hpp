#pragma once

extern "C"
{
#include "v2/_interface/RectC_def.h"
}

namespace _Core
{
	template<typename T>
	struct VectorT;
}

namespace _MathV2
{
	template<int C, typename T>
	struct Vector;
}

namespace _RenderV2
{
	template<int C, typename T>
	struct Texture;

	struct RasterizationStep;
}

namespace _RenderV2
{
	struct DrawM
	{
		static	void DrawLineClipped(
			_MathV2::Vector<2, float>* p_begin, _MathV2::Vector<2, float>* p_end,
			_Core::VectorT<RasterizationStep>* in_out_rasterizedPixelsBuffer,
			Texture<3, char>* p_to, RECTI_PTR p_clipRect,
			_MathV2::Vector<3, char>* p_color);
	};
}