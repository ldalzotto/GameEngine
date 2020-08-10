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
	template <int C, typename T>
	struct Vector;
	struct Box;
}

namespace _RenderV2
{
	template <int C, typename T>
	struct Texture;

	struct Mesh;
}

namespace _RenderV2
{

	struct RenderableObject
	{
		Mesh* Mesh;
		_MathV2::Box* MeshBoundingBox;
		_MathV2::Matrix<4, 4, float>* ModelMatrix;
	};

	struct RendererConfiguration
	{
		bool ObjectCullEnabled;
	};

	struct WireframeRendererInput
	{
		RendererConfiguration RendererConfiguration;
		_Core::VectorT<RenderableObject>* RenderableObjects;
		_MathV2::Matrix<4, 4, float>* ViewMatrix;
		_MathV2::Matrix<4, 4, float>* ProjectionMatrix;
		_MathV2::Matrix<4, 4, float>* GraphicsAPIToScreeMatrix;
		_MathV2::Vector<4, float>* CameraWorldPosition;	
	};

	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to);
}