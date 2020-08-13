#pragma once

#include "v2/Vector/Vector.hpp"

namespace _Core
{
	template <typename T>
	struct VectorT;

	template <typename T>
	struct ArraySliceT;
}

namespace _RenderV2
{
	struct LineRasterizationInterpolation
	{
		float L1, L2;
	};

	struct Rasterizer
	{
		static void line_v3(const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, _Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels);
		static void line_v3_clipped(
			const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end,
			_Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels,
			const int p_texWidth, const int p_texHeight);
		/*
		static void line(
			const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, 
			const _MathV2::Vector<4, float>* p_begin_cameraSpace, const _MathV2::Vector<4, float>* p_end_cameraSpace,
			bool l_interpolate, _Core::VectorT<LineRasterizationResult>* out_rasterizedPixels
		);
		*/
	};
}