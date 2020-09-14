#pragma once


#include "Raster/Rasterizer_def.h"
#include "Objects/RenderedObject.h"
#include "Objects/Resource/Polygon_def.h"
#include "v2/_interface/MatrixC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/WindowSize_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.h"
#include "DataStructures/ARRAY_def.h"

typedef struct WireframeRendererInput_TYP
{
	RENDEREDOBJECT_BUFFER_PTR RenderableObjectsBuffer;
	CAMERABUFFER_PTR CameraBuffer;
	WindowSize WindowSize;
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
	size_t* PolygonPipelineSize;
	Array_Char IsCulled;
	Array_Polygon_VertexIndex VerticesIndex;
}PolygonPipelineV2, * PolygonPipelineV2_PTR;

typedef struct VertexPipeline_TYP
{
	size_t* VertexPipelineSize;
	Array_VertexHANDLE Vertex;
	Array_Vector4f CameraSpacePosition;
	Array_Vector4f TransformedPosition;
	Array_Vector2i PixelPosition;
	Array_Char PixelPositionCalculated;
}VertexPipeline, * VertexPipeline_PTR;

typedef struct WireframeRenderer_Memory_TYP
{
	Array_RenderableObjectPipeline RederableObjectsPipeline;
	PolygonPipelineV2 PolygonPipelines;
	VertexPipeline VertexPipeline;
	ARRAY_RASTERISATIONSTEP RasterizedPixelsBuffer;
} WireframeRenderer_Memory, * WireframeRenderer_Memory_PTR;

void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory);
void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height);
void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory);

void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture3c_PTR p_to, Recti_PTR p_to_clipRect, WireframeRenderer_Memory* p_memory);
