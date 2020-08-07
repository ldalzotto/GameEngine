#pragma once

namespace _Core
{
	template <typename T>
	struct VectorT;
}

namespace _MathV2
{
	template<int C, typename T>
	struct Vector;
}

namespace _RenderV2
{
	struct Rasterizer
	{
		static void line(const _MathV2::Vector<2, float>* p_begin, const _MathV2::Vector<2, float>* p_end, _Core::VectorT<_MathV2::Vector<2, int>>* out_rasterizedPixels);
	};
}