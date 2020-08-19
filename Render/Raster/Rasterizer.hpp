#pragma once

#include "v2/Vector/Vector.hpp"
extern "C"
{
#include "v2/_interface/RectC_def.h"
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
			const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end,
			_Core::VectorT<RasterizationStep>* out_rasterizedPixels,
			const RECTI_PTR p_clip_rect,
			_MathV2::Vector<2, int>* out_clipped_begin,
			_MathV2::Vector<2, int>* out_clipped_end);
	};
}