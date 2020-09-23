#include "RendererPipelineMemory.h"

#include "DataStructures/ARRAY.h"
#include "Metrics/RenderTimeMetrics.h"

ARRAY_ALLOC_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)

ARRAY_ALLOC_FUNCTION(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)

ARRAY_ALLOC_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)

ARRAY_ALLOC_FUNCTION(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)



void RendererPipeline_Memory_Alloc(RendererPipeline_Memory_PTR p_memory)
{
	Arr_Alloc_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline, 0);
	Arr_Alloc_VertexPipeline(&p_memory->VertexPipeline, 0);
	Arr_Alloc_PolygonPipelineV2(&p_memory->PolygonPipelines, 0);
	Arr_Alloc_PolygonPipeline_CameraDistanceIndexed(&p_memory->OrderedPolygonPipelinesIndex, 0);

#if HAS_OPENCL
	Arr_Alloc_PolygonPipeline_CameraDistanceIndexed(&p_memory->PolygonPipelinesIndexNotFiltered, 0);
#endif
};

void RendererPipeline_Memory_Clear(RendererPipeline_Memory_PTR p_memory)
{
	Arr_Clear(&p_memory->RederableObjectsPipeline.array);
	Arr_Clear(&p_memory->VertexPipeline.array);
	Arr_Clear(&p_memory->PolygonPipelines.array);
	Arr_Clear(&p_memory->OrderedPolygonPipelinesIndex.array);
};

void RendererPipeline_Memory_Free(RendererPipeline_Memory_PTR p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	SolidRendererMetrics_Print(&GWireframeRendererPerformace);
#endif

	Arr_Free(&p_memory->RederableObjectsPipeline.array);
	Arr_Free(&p_memory->VertexPipeline.array);
	Arr_Free(&p_memory->PolygonPipelines.array);
	Arr_Free(&p_memory->OrderedPolygonPipelinesIndex.array);

#if HAS_OPENCL
	Arr_Free(&p_memory->PolygonPipelinesIndexNotFiltered.array);
#endif
};

