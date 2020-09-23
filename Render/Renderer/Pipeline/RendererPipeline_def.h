#pragma once

#include "Objects/RenderedObject_def.h"
#include "Objects/Resource/Material_def.h"
#include "Objects/Resource/Mesh_def.h"

typedef struct RenderableObjectPipeline_TYP
{
	RenderedObject_HANDLE RenderedObject;

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
	Polygon_VertexIndex VerticesPipelineIndex;

	Vector3f CalculatedFlatWorldNormal;

	size_t AssociatedRenderableObjectPipeline;
	size_t MeshPolygonIndex;
	Material_HANDLE Material;
}PolygonPipelineV2, * PolygonPipelineV2_PTR;

typedef struct ARRAY_PolygonPipelineV2_TYP
{
	ARRAY_TYPE_DEFINITION(PolygonPipelineV2)
} ARRAY_PolygonPipelineV2, * ARRAY_PolygonPipelineV2_PTR;

typedef size_t PolygonPipelineIndex;

typedef struct PolygonPipeline_CameraDistanceIndexed_TYP
{
	size_t Index;
	float DistanceFromCamera;
}PolygonPipeline_CameraDistanceIndexed, * PolygonPipeline_CameraDistanceIndexed_PTR;

typedef struct ARRAY_PolygonPipeline_CameraDistanceIndexed_TYP
{
	ARRAY_TYPE_DEFINITION(PolygonPipeline_CameraDistanceIndexed)
}ARRAY_PolygonPipeline_CameraDistanceIndexed, * ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR;

typedef struct VertexPipeline_TYP
{
	Vertex_HANDLE Vertex;
	size_t AssociatedRenderableObjectPipeline;
	Vector4f WorldPosition;
	Vector4f CameraSpacePosition;
	Vector2i PixelPosition;
	char WorldPositionCalculated;
	char PixelPositionCalculated;
}VertexPipeline, * VertexPipeline_PTR;

typedef struct Array_VertexPipeline_TYP
{
	ARRAY_TYPE_DEFINITION(VertexPipeline)
}Array_VertexPipeline, * Array_VertexPipeline_PTR;


typedef struct RendererPipeline_Memory_TYP
{
	Array_RenderableObjectPipeline RederableObjectsPipeline;
	ARRAY_PolygonPipelineV2 PolygonPipelines;
	Array_VertexPipeline VertexPipeline;

#if HAS_OPENCL
	ARRAY_PolygonPipeline_CameraDistanceIndexed PolygonPipelinesIndexNotFiltered;
#endif
	ARRAY_PolygonPipeline_CameraDistanceIndexed OrderedPolygonPipelinesIndex;
} RendererPipeline_Memory, * RendererPipeline_Memory_PTR;