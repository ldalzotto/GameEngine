#pragma once


#include "Raster/Rasterizer_def.h"
#include "Objects/RenderedObject.h"
#include "Objects/Resource/Polygon_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "DataStructures/ARRAY_def.h"

typedef struct WireframeRendererInput_TYP
{
	RENDEREDOBJECT_BUFFER_PTR RenderableObjectsBuffer;
	CAMERABUFFER_PTR CameraBuffer;
	Matrix4f_PTR GraphicsAPIToScreeMatrix;
} WireframeRendererInput, * WireframeRendererInput_PTR;


typedef struct RenderableObjectPipeline_TYP
{
	RenderedObject_PTR RenderedObject;

	size_t PolygonPipelineIndexBeginIncluded;
	size_t PolygonPipelineIndexEndExcluded;

	size_t VertexPipelineIndexBeginIncluded;
	size_t VertexPipelineIndexEndExcluded;
} RenderableObjectPipeline, * RenderableObjectPipeline_PTR;

typedef struct Array_RenderableObjectPipeline_TYP
{
	ARRAY_TYPE_DEFINITION(RenderableObjectPipeline)
}Array_RenderableObjectPipeline, * Array_RenderableObjectPipeline_PTR;

typedef struct PolygonPipelineV2_TYP
{
	char IsCulled;
	Polygon_VertexIndex VerticesIndex;
}PolygonPipelineV2, * PolygonPipelineV2_PTR;

typedef struct ARRAY_PolygonPipelineV2_TYP
{
	ARRAY_TYPE_DEFINITION(PolygonPipelineV2)
} ARRAY_PolygonPipelineV2, * ARRAY_PolygonPipelineV2_PTR;

typedef struct VertexPipeline_TYP
{
	Vector4f CameraSpacePosition;
	Vector4f TransformedPosition;
	Vector2i PixelPosition;
	char PixelPositionCalculated;
}VertexPipeline, * VertexPipeline_PTR;

typedef struct Array_VertexPipeline_TYP
{
	ARRAY_TYPE_DEFINITION(VertexPipeline)
}Array_VertexPipeline, * Array_VertexPipeline_PTR;

typedef struct WireframeRenderer_Memory_TYP
{
	Array_RenderableObjectPipeline RederableObjectsPipeline;
	ARRAY_PolygonPipelineV2 PolygonPipelines;
	Array_VertexPipeline VertexPipeline;
	ARRAY_RASTERISATIONSTEP RasterizedPixelsBuffer;
} WireframeRenderer_Memory, * WireframeRenderer_Memory_PTR;

void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory);
void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height);
void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory);

void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture3c_PTR p_to, Recti_PTR p_to_clipRect, WireframeRenderer_Memory* p_memory);
