#pragma once

#include "v2/Vector/Vector.hpp"

namespace _Core
{
	template <typename T>
	struct VectorT;

	template <typename T>
	struct ArraySliceT;
}

namespace _MathV2
{
	template<typename T>
	struct Rect;
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
			const _MathV2::Rect<int>* p_clip_rect,
			_MathV2::Vector<2, int>* out_clipped_begin,
			_MathV2::Vector<2, int>* out_clipped_end);
	};
}