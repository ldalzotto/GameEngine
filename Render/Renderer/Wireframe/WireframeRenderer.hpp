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
	template <int C, typename T>
	struct Texture;
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
		_MathV2::Matrix<4, 4, float>* GraphicsAPIToScreeMatrix;
	};

	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to);
}