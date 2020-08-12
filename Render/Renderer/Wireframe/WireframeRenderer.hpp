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

	struct PolygonPipeline
	{
		RenderableObjectPipeline* RenderedObject;
		size_t PolygonIndex;

		bool IsCulled;

		Polygon<_MathV2::Vector<4, float>> CameraSpacePolygon;
		Polygon<_MathV2::Vector<4, float>> TransformedPolygon;
	};

	struct LineInterpolationFactor
	{
		float Interpolation_0, Interpolation_1;
	};

	struct PixelsPipeline
	{
		_Core::ArraySliceT<_MathV2::Vector<2, int>> ScreenPosition;
		_Core::ArraySliceT<LineInterpolationFactor> Interpolation;
	};

	struct LineRasterisationResult
	{
		PolygonPipeline* UsedPolygon;
		short int Vertex1_Index; short int Vertex2_Index;
		PixelsPipeline PixelsToDraw;
	};

	struct WireframeRenderMemory
	{
		_Core::VectorT<RenderableObjectPipeline> RenderableObjectsPipeline;
		_Core::VectorT<PolygonPipeline> PolygonPipeline;

		_Core::VectorT<_MathV2::Vector<2, int>> PixelDrawnCoordsBufferV2;

		_Core::VectorT<_MathV2::Vector<2, int>> PixelsToDrawV2_ScreenPosition;
		_Core::VectorT<LineInterpolationFactor> PixelsToDrawV2_Interpolation;

		_Core::VectorT<LineRasterisationResult> LineRasterisation;
	};

	void WireframeRenderMemory_alloc(WireframeRenderMemory* p_memory);
	void WireframeRenderMemory_clear(WireframeRenderMemory* p_memory);
	void WireframeRenderMemory_free (WireframeRenderMemory* p_memory);

	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to, WireframeRenderMemory* p_memory);
}