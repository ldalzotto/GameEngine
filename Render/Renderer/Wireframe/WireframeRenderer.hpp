#pragma once

#include "Objects/Polygon.hpp"

namespace _Core
{
	template <typename T>
	struct VectorT;
}

namespace _MathV2
{
	template <int C, int L, typename T>
	struct Matrix;
}

namespace _RenderV2
{

	struct PolygonInput
	{
		Polygon<Vertex*>* Polygon;
		_MathV2::Matrix<4, 4, float>* ModelMatrix;
	};

	struct WireframeRendererInput
	{
		_Core::VectorT<PolygonInput>* Polygons;
		_MathV2::Matrix<4, 4, float>* ViewMatrix;
		_MathV2::Matrix<4, 4, float>* ProjectionMatrix;
	};

	void WirerameRenderer_render(WireframeRendererInput* p_input);
}