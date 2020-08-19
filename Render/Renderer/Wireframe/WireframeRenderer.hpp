#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Raster/Rasterizer.hpp"

extern "C"
{
#include "Objects/Resource/Polygon_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
}

namespace _RenderV2
{
	struct Texture3C;

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

		POLYGON4F CameraSpacePolygon;
		POLYGON4F TransformedPolygon;
		POLYGON2F PixelPolygon;
	};

	struct WireframeRenderer_Memory
	{
		_Core::VectorT<PolygonPipelineV2> PolygonPipelines;
		_Core::VectorT<RasterizationStep> RasterizedPixelsBuffer;
	};

	void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory);
	void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height);
	void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory);

	void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture3C* p_to, RECTI_PTR p_to_clipRect, WireframeRenderer_Memory* p_memory);
}