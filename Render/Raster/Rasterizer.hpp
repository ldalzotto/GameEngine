#pragma once

extern "C"
{
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
}

namespace _Core
{
	template <typename T>
	struct VectorT;

	template <typename T>
	struct ArraySliceT;
}

namespace _RenderV2
{
	enum class RasterizationStepDirection : short int
	{
		NONE = 0,
		ADD = 1,
		REMOVE = 2
	};

	struct RasterizationStep
	{
		RasterizationStepDirection XDirection;
		RasterizationStepDirection YDirection;
	};


	struct Rasterizer
	{
		static bool line_v3_clipped(
			const VECTOR2F_PTR p_begin, const VECTOR2F_PTR p_end,
			_Core::VectorT<RasterizationStep>* out_rasterizedPixels,
			const RECTI_PTR p_clip_rect,
			VECTOR2I_PTR out_clipped_begin,
			VECTOR2I_PTR out_clipped_end);
	};
}