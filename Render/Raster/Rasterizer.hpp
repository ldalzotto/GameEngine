#pragma once

#include "v2/Vector/Vector.hpp"

namespace _Core
{
	template <typename T>
	struct VectorT;
}

namespace _RenderV2
{
	struct LineRasterizationResult
	{
		_MathV2::Vector<2, int> PixelCoord;
		float L1, L2;
	};

	struct Rasterizer
	{
		static void line(
			const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, 
			const _MathV2::Vector<4, float>* p_begin_cameraSpace, const _MathV2::Vector<4, float>* p_end_cameraSpace,
			bool l_interpolate, _Core::VectorT<LineRasterizationResult>* out_rasterizedPixels
		);
	};
}