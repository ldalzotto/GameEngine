#pragma once

#include "Objects/Resource/Polygon.hpp"
#include "v2/Vector/Vector.hpp"
#include "DataStructures/Specifications/ArraySliceT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

namespace _MathV2
{
	template <int C, int L, typename T>
	struct Matrix;
}

namespace _RenderV2
{
	template <int C, typename T>
	struct Texture;

	struct Mesh;
	struct LineRasterizationResult;
	struct RenderedObject;
}

namespace _RenderV2
{
	struct RendererConfiguration
	{
		bool ObjectCullEnabled;
	};

	struct WireframeRendererInput
	{
		RendererConfiguration RendererConfiguration;
		_Core::VectorT<RenderedObject>* RenderableObjects;
		_MathV2::Matrix<4, 4, float>* ViewMatrix;
		_MathV2::Matrix<4, 4, float>* ProjectionMatrix;
		_MathV2::Matrix<4, 4, float>* GraphicsAPIToScreeMatrix;
		_MathV2::Vector<4, float>* CameraWorldPosition;	
	};


	struct RenderableObjectPipeline
	{
		bool IsCulled;
		RenderedObject* RenderableObject;
	};

	struct PolygonPipelineV2
	{
		RenderedObject* RenderedObject;
		size_t PolygonIndex;

		Polygon<_MathV2::Vector<4, float>> CameraSpacePolygon;
		Polygon<_MathV2::Vector<4, float>> TransformedPolygon;
	};

	struct WireframeRenderer_Memory
	{
		_Core::VectorT<PolygonPipelineV2> PolygonPipelines;
		_Core::VectorT<_MathV2::Vector<2, int>> RasterizedPixelsBuffer;
		_Core::VectorT<bool> RasterizerBufferV2;
	};

	void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory);
	void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_rasterizerBufferSize);
	void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory);

	void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture<3, char>* p_to, WireframeRenderer_Memory* p_memory);
}