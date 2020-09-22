#include "SolidRenderer.h"

#include <stdio.h>

#include "Heap/RenderHeap.h"
#include "Renderer/Transformation/RenderableObjectTransform.h"
#include "Objects/Resource/Polygon.h"
#include "Cull/ObjectCulling.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorStructuresC.h"
#include "Renderer/Draw/FlatShade.h"
#include "Metrics/RenderTimeMetrics.h"
#include "Algorithm/Sort_alg.h"

ARRAY_ALLOC_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)

ARRAY_ALLOC_FUNCTION(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)

ARRAY_ALLOC_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)
ARRAY_RESIZE_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)

ARRAY_ALLOC_FUNCTION(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)

// There is a unique directional light
RenderLights GRenderLights =
{
	.DirectionalLight = {.Direction = {1.0f, 0.0f, 0.0f}, .Intensity = 1.0f, .Color = {1.0f, 1.0f, 1.0f} },
	.AmbientLight = {.Color = {0.5f, 0.5f, 0.5f} }
};

void SolidRenderer_renderV2(const SolidRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision l_wireframeRenderBegin = Clock_currentTime_mics();
#endif

	SolidRenderer_Memory_clear(p_memory, p_to->Width, p_to->Height);

#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision tmp_timer, tmp_timer_2;
	tmp_timer = Clock_currentTime_mics();
#endif

	for (size_t i = 0; i < p_input->RenderableObjectsBuffer->RenderedObjects.Size; i++)
	{
		RenderedObject_PTR l_renderableObject = p_input->RenderableObjectsBuffer->RenderedObjects.Memory[i];
		Matrix4f l_object_to_camera;
		Mat_Mul_M4F_M4F((Matrix4f_PTR)p_input->CameraBuffer->ViewMatrix, &l_renderableObject->ModelMatrix, &l_object_to_camera);

		if (!ObjectCulled_Boxf(l_renderableObject->MeshBoundingBox, (Matrix4f_PTR)&l_renderableObject->ModelMatrix, (Matrix4f_PTR)&l_object_to_camera, p_input->CameraBuffer->CameraFrustum))
		{
#if RENDER_PERFORMANCE_TIMER
			tmp_timer_2 = Clock_currentTime_mics();
#endif

			size_t l_vertexIndexOffset = p_memory->VertexPipeline.Size;
			for (size_t j = 0; j < l_renderableObject->Mesh->Vertices.Size; j++)
			{
				Arr_PushBackRealloc_Empty_VertexPipeline(&p_memory->VertexPipeline);
				p_memory->VertexPipeline.Memory[p_memory->VertexPipeline.Size - 1] = (VertexPipeline){
					.Vertex = l_renderableObject->Mesh->Vertices.Memory[j],
					.WorldPositionCalculated = 0,
					.PixelPositionCalculated = 0
				};
			}

			size_t l_polygonPipelineIndexOffset = p_memory->PolygonPipelines.Size;

			Material* l_renderableObjectMaterial = &RRenderHeap.MaterialAllocator.array.Memory[l_renderableObject->Material.Handle];

			for (size_t j = 0; j < l_renderableObject->Mesh->Polygons.Size; j++)
			{
				Polygon_VertexIndex_PTR l_polygon = &RRenderHeap.PolygonAllocator.array.Memory[l_renderableObject->Mesh->Polygons.Memory[j].Handle];

				Arr_PushBackRealloc_Empty_PolygonPipelineV2(&p_memory->PolygonPipelines);
				PolygonPipelineV2* l_polygonPipeline = &p_memory->PolygonPipelines.Memory[p_memory->PolygonPipelines.Size - 1];
				*l_polygonPipeline = (PolygonPipelineV2){
										.IsCulled = 0,
										.VerticesPipelineIndex = {
											l_polygon->v1 + l_vertexIndexOffset,
											l_polygon->v2 + l_vertexIndexOffset,
											l_polygon->v3 + l_vertexIndexOffset
										},
										.AssociatedRenderableObjectPipeline = p_memory->RederableObjectsPipeline.Size,
										.Material = l_renderableObject->Material,
										.MeshPolygonIndex = j
				};
			}

			Arr_PushBackRealloc_Empty_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline);
			p_memory->RederableObjectsPipeline.Memory[p_memory->RederableObjectsPipeline.Size - 1] = (RenderableObjectPipeline){
				.RenderedObject = l_renderableObject,
				.PolygonPipelineIndexBeginIncluded = l_polygonPipelineIndexOffset,
				.PolygonPipelineIndexEndExcluded = p_memory->PolygonPipelines.Size,
				.VertexPipelineIndexBeginIncluded = l_vertexIndexOffset,
				.VertexPipelineIndexEndExcluded = p_memory->VertexPipeline.Size
			};

#if RENDER_PERFORMANCE_TIMER
			PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageDataSetup_PushPipelineData, Clock_currentTime_mics() - tmp_timer_2);
#endif

		}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageDataSetup, Clock_currentTime_mics() - tmp_timer);
#endif


	RenderableObjectTransform_Input l_renderableObjectTransformInput = 
	{
		.RenderHeap = &RRenderHeap,
		.RendererPipelineMemory = p_memory,
		.CameraBuffer = p_input->CameraBuffer,
		.WindowSize = &p_input->WindowSize
	};
	RendereableObject_TransformPolygons(&l_renderableObjectTransformInput);


#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	DrawPolygFlatShadeTexturedInput l_drawInput = {
		.DepthBuffer = p_depthBuffer,
		.RendererPipelineMemory = p_memory,
		.RenderHeap = &RRenderHeap,
		.RenderLights = &GRenderLights,
		.RenderTarget = p_to,
		.TargetClip = p_to_clipRect
	};
	DrawPoly_FlatShade_Textured(&l_drawInput);


#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_IncrementCounter(&GWireframeRendererPerformace.AverageRasterization_TransformCoords);
#endif
#if RENDER_PERFORMANCE_TIMER && RENDER_PERFORMANCE_TIMER_PER_PIXEL
	PerformanceCounter_IncrementCounter(&GWireframeRendererPerformace.AverageRasterization_PolygonRasterize);
	PerformanceCounter_IncrementCounter(&GWireframeRendererPerformace.AverageRasterization_PixelShading);
#endif
#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRasterization, Clock_currentTime_mics() - tmp_timer);
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRender, Clock_currentTime_mics() - l_wireframeRenderBegin);
#endif
};

void SolidRenderer_Memory_alloc(RendererPipeline_Memory_PTR p_memory)
{
	Arr_Alloc_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline, 0);
	Arr_Alloc_VertexPipeline(&p_memory->VertexPipeline, 0);
	Arr_Alloc_PolygonPipelineV2(&p_memory->PolygonPipelines, 0);
	Arr_Alloc_PolygonPipeline_CameraDistanceIndexed(&p_memory->OrderedPolygonPipelinesIndex, 0);
};
void SolidRenderer_Memory_clear(RendererPipeline_Memory_PTR p_memory, size_t p_width, size_t height)
{
	Arr_Clear(&p_memory->RederableObjectsPipeline.array);
	Arr_Clear(&p_memory->VertexPipeline.array);
	Arr_Clear(&p_memory->PolygonPipelines.array);
	Arr_Clear(&p_memory->OrderedPolygonPipelinesIndex.array);
};

void SolidRenderer_Memory_free(RendererPipeline_Memory_PTR p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	SolidRendererMetrics_Print(&GWireframeRendererPerformace);
#endif

	Arr_Free(&p_memory->RederableObjectsPipeline.array);
	Arr_Free(&p_memory->VertexPipeline.array);
	Arr_Free(&p_memory->PolygonPipelines.array);
	Arr_Free(&p_memory->OrderedPolygonPipelinesIndex.array);
};