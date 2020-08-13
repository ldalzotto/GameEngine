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
	struct LineRasterizationInterpolation
	{
		float L1, L2;
	};

	struct Rasterizer
	{
		static void line_v3_clipped(
			const _MathV2::Vector<2, int>* p_begin, const _MathV2::Vector<2, int>* p_end,
			_Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels,
			const _MathV2::Rect<int>* p_clip_rect);
		/*
		static void line(
			const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, 
			const _MathV2::Vector<4, float>* p_begin_cameraSpace, const _MathV2::Vector<4, float>* p_end_cameraSpace,
			bool l_interpolate, _Core::VectorT<LineRasterizationResult>* out_rasterizedPixels
		);
		*/
	};
}