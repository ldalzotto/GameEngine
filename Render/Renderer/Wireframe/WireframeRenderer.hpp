#pragma once

#include "Objects/Resource/Polygon.hpp"
#include "v2/Vector/Vector.hpp"
#include "DataStructures/Specifications/VectorT.hpp"
#include "Raster/Rasterizer.hpp"

extern "C"
{
#include "v2/_interface/MatrixC_def.h"
}

namespace _MathV2
{
	template<typename T>
	struct Rect;
}

namespace _RenderV2
{
	template <int C, typename T>
	struct Texture;

	struct RenderedObject;

	struct CameraBuffer;
	struct RenderedObjectsBuffer;
}

namespace _RenderV2
{
	struct WireframeRendererInput
	{
		RenderedObjectsBuffer* RenderableObjectsBuffer;
		CameraBuffer* CameraBuffer;
		MATRIX4F_PTR GraphicsAPIToScreeMatrix;
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
		Polygon<_MathV2::Vector<2, float>> PixelPolygon;
	};

	struct WireframeRenderer_Memory
	{
		_Core::VectorT<PolygonPipelineV2> PolygonPipelines;
		_Core::VectorT<RasterizationStep> RasterizedPixelsBuffer;
	};

	void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory);
	void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height);
	void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory);

	void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture<3, char>* p_to, _MathV2::Rect<int>* p_to_clipRect, WireframeRenderer_Memory* p_memory);
}