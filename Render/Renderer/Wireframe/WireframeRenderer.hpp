#pragma once

#include "Objects/Polygon.hpp"
#include "v2/Vector/Vector.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

namespace _MathV2
{
	template <int C, int L, typename T>
	struct Matrix;
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


	struct RenderableObjectPipeline
	{
		bool IsCulled;
		RenderableObject* RenderableObject;
	};

	struct PolygonPipeline
	{
		bool IsCulled;
		Polygon<_MathV2::Vector<4, float>> TransformedPolygon;
		_MathV2::Matrix<4, 4, float>* ModelMatrix;
	};


	struct WireframeRenderMemory
	{
		_Core::VectorT<RenderableObjectPipeline> RenderableObjectsPipeline;
		_Core::VectorT<PolygonPipeline> PolygonPipeline;

		_Core::VectorT<_MathV2::Vector2<int>> PixelDrawnCoordsBuffer;
		_Core::VectorT<_MathV2::Vector2<int>> PixelsDrawn;
		_Core::VectorT<_MathV2::Vector3<char>> PixelsDrawnColors;
	};

	void WireframeRenderMemory_alloc(WireframeRenderMemory* p_memory);
	void WireframeRenderMemory_clear(WireframeRenderMemory* p_memory);
	void WireframeRenderMemory_free (WireframeRenderMemory* p_memory);

	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to, WireframeRenderMemory* p_memory);
}