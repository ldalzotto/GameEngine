#pragma once

namespace _RenderV2
{
	template<typename T>
	struct Polygon;
}

namespace _MathV2
{
	template<int C, typename T>
	struct Vector;
}

namespace _RenderV2
{
	struct BackfaceCullingM
	{
		static bool isCulled(const Polygon<_MathV2::Vector<4, float>>* p_polygon, const _MathV2::Vector<4, float>* p_cameraWorldPosition);
	};
}