#include "WireframeRenderer.h"

#include <stdio.h>

#include "Heap/RenderHeap.h"
#include "Cull/ObjectCulling.h"
#include "Cull/BackfaceCulling.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/WindowSize.h"
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorStructuresC.h"
#include "Raster/Rasterizer.h"
#include "Renderer/Draw/DrawFunctions.h"

ARRAY_ALLOC_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)

ARRAY_ALLOC_FUNCTION(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)

ARRAY_ALLOC_FUNCTION(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)
ARRAY_PUSHBACKREALLOC_FUNCTION_PTR(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)

#if RENDER_PERFORMANCE_TIMER
#include "Clock/Clock.h"

typedef struct PerformanceCounter_TYP
{
	size_t SampleCount;
	TimeClockPrecision AccumulatedTime;
}PerformanceCounter, * PerformanceCounter_PTR;

void PerformanceCounter_PushSample(PerformanceCounter_PTR p_performanceCounter, TimeClockPrecision p_sampleTime);

typedef struct WireframeRendererPerformace_TYP
{
	PerformanceCounter AverageRender;
	PerformanceCounter AverageDataSetup;
	PerformanceCounter AverageLocalToWorld;
	PerformanceCounter AverageBackfaceCulling;
	PerformanceCounter AverageRasterization;
	PerformanceCounter AverageRasterization_PixelDrawing;
}WireframeRendererPerformace, * WireframeRendererPerformace_PTR;

WireframeRendererPerformace GWireframeRendererPerformace = { 0 };

void WireframeRendererPerformace_Print(WireframeRendererPerformace_PTR p_wireframeRenderPerformance);

#endif

inline void WireframeRenderer_CalculatePixelPosition_FromWorldPosition(VertexPipeline_PTR p_vertex, const WireframeRendererInput* p_input)
{
	if (!p_vertex->PixelPositionCalculated)
	{
		// World to camera
		Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &p_vertex->TransformedPosition, &p_vertex->CameraSpacePosition);

		// Camera to clip
		Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &p_vertex->CameraSpacePosition, &p_vertex->TransformedPosition);

		// To pixel
		WindowSize_GraphicsAPIToPixel(&p_input->WindowSize, p_vertex->TransformedPosition.x, p_vertex->TransformedPosition.y, &p_vertex->PixelPosition.x, &p_vertex->PixelPosition.y);

		p_vertex->PixelPositionCalculated = 1;
	}
};

void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture3c_PTR p_to, Recti_PTR p_to_clipRect, WireframeRenderer_Memory* p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision l_wireframeRenderBegin = Clock_currentTime_mics();
#endif

	WireframeRenderer_Memory_clear(p_memory, p_to->Width, p_to->Height);
	Vector3c l_wireframeColor = { 255,0,0 };
	Vector4f tmp_vec4_0;

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
			VertexPipeline l_vertexPipeline;
			size_t l_vertexIndexOffset = p_memory->VertexPipeline.Size;
			for (size_t j = 0; j < l_renderableObject->Mesh->Vertices.Size; j++)
			{
				l_vertexPipeline = (VertexPipeline){
					.Vertex = l_renderableObject->Mesh->Vertices.Memory[j],
					.PixelPositionCalculated = 0
				};
				Arr_PushBackRealloc_VertexPipeline(&p_memory->VertexPipeline, &l_vertexPipeline);
			}

			PolygonPipelineV2 l_polygonPipeline;
			size_t l_polygonPipelineIndexOffset = p_memory->PolygonPipelines.Size;
			for (size_t j = 0; j < l_renderableObject->Mesh->Polygons.Size; j++)
			{
				Polygon_VertexIndex_PTR l_polygon = &RRenderHeap.PolygonAllocator.array.Memory[l_renderableObject->Mesh->Polygons.Memory[j].Handle];

				l_polygonPipeline = (PolygonPipelineV2){
					.IsCulled = 0,
					.VerticesIndex = {
						l_polygon->v1 + l_vertexIndexOffset,
						l_polygon->v2 + l_vertexIndexOffset,
						l_polygon->v3 + l_vertexIndexOffset
					}
				};

				Arr_PushBackRealloc_PolygonPipelineV2(&p_memory->PolygonPipelines, &l_polygonPipeline);
			}


			RenderableObjectPipeline l_renderaBleObjectPipeline = {
				.RenderedObject = l_renderableObject,
				.PolygonPipelineIndexBeginIncluded = l_polygonPipelineIndexOffset,
				.PolygonPipelineIndexEndExcluded = p_memory->PolygonPipelines.Size,
				.VertexPipelineIndexBeginIncluded = l_vertexIndexOffset,
				.VertexPipelineIndexEndExcluded = p_memory->VertexPipeline.Size
			};

			Arr_PushBackRealloc_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline, &l_renderaBleObjectPipeline);

		}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageDataSetup, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	// Local to world
	for (size_t i = 0; i < p_memory->RederableObjectsPipeline.Size; i++)
	{
		RenderableObjectPipeline_PTR l_renderableObject = &p_memory->RederableObjectsPipeline.Memory[i];

		for (size_t j = l_renderableObject->VertexPipelineIndexBeginIncluded; j < l_renderableObject->VertexPipelineIndexEndExcluded; j++)
		{
			VertexPipeline_PTR l_vertexPipeline = &p_memory->VertexPipeline.Memory[j];
			Vertex_PTR l_vertex = &RRenderHeap.VertexAllocator.array.Memory[l_vertexPipeline->Vertex.Handle];
			Mat_Mul_M4F_V4F(&l_renderableObject->RenderedObject->ModelMatrix, &l_vertex->LocalPosition, &tmp_vec4_0);
			l_vertexPipeline->TransformedPosition = tmp_vec4_0;
		}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageLocalToWorld, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	// Backface culling
	for (size_t i = 0; i < p_memory->PolygonPipelines.Size; i++)
	{
		PolygonPipelineV2_PTR l_polygon = &p_memory->PolygonPipelines.Memory[i];
		Polygon4fPTR l_poly =
		{
			.v1 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesIndex.v1].TransformedPosition,
			.v2 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesIndex.v2].TransformedPosition,
			.v3 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesIndex.v3].TransformedPosition
		};

		l_polygon->IsCulled = BackFaceCulled_Poly4FPTR(&l_poly, &p_input->CameraBuffer->WorldPosition);
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageBackfaceCulling, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	for (size_t i = 0; i < p_memory->PolygonPipelines.Size; i++)
	{
		PolygonPipelineV2_PTR l_polygon = &p_memory->PolygonPipelines.Memory[i];

		if (!l_polygon->IsCulled)
		{
			VertexPipeline_PTR l_v1 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesIndex.v1];
			VertexPipeline_PTR l_v2 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesIndex.v2];
			VertexPipeline_PTR l_v3 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesIndex.v3];

			WireframeRenderer_CalculatePixelPosition_FromWorldPosition(l_v1, p_input);
			WireframeRenderer_CalculatePixelPosition_FromWorldPosition(l_v2, p_input);
			WireframeRenderer_CalculatePixelPosition_FromWorldPosition(l_v3, p_input);

#if RENDER_PERFORMANCE_TIMER
			tmp_timer_2 = Clock_currentTime_mics();
#endif
			Polygon2i l_polygon = {
				.v1 = l_v1->PixelPosition,
				.v2 = l_v2->PixelPosition,
				.v3 = l_v3->PixelPosition
			};
			Draw_PolygonClipped(&l_polygon, p_to, p_to_clipRect, &l_wireframeColor);
			
#if RENDER_PERFORMANCE_TIMER
			PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRasterization_PixelDrawing, Clock_currentTime_mics() - tmp_timer);
#endif

		}

	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRasterization, Clock_currentTime_mics() - tmp_timer);
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageRender, Clock_currentTime_mics() - l_wireframeRenderBegin);
	WireframeRendererPerformace_Print(&GWireframeRendererPerformace);
#endif
};


void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory)
{
	Arr_Alloc_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline, 0);
	Arr_Alloc_VertexPipeline(&p_memory->VertexPipeline, 0);
	Arr_Alloc_PolygonPipelineV2(&p_memory->PolygonPipelines, 0);

};
void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height)
{
	Arr_Clear(&p_memory->RederableObjectsPipeline.array);
	Arr_Clear(&p_memory->VertexPipeline.array);
	Arr_Clear(&p_memory->PolygonPipelines.array);
};
void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory)
{
	Arr_Free(&p_memory->RederableObjectsPipeline.array);
	Arr_Free(&p_memory->VertexPipeline.array);
	Arr_Free(&p_memory->PolygonPipelines.array);
};

#if RENDER_PERFORMANCE_TIMER
void PerformanceCounter_PushSample(PerformanceCounter_PTR p_performanceCounter, TimeClockPrecision p_sampleTime)
{
	p_performanceCounter->SampleCount += 1;
	p_performanceCounter->AccumulatedTime += p_sampleTime;
};

TimeClockPrecision PerformanceCounter_GetAverage(PerformanceCounter_PTR p_performanceCounter)
{
	return p_performanceCounter->AccumulatedTime / p_performanceCounter->SampleCount;
};

void WireframeRendererPerformace_Print(WireframeRendererPerformace_PTR p_wireframeRenderPerformance)
{
	printf("WireframeRenderer_renderV2 : %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRender));
	printf("  -> Data Setup %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageDataSetup));
	printf("  -> Local To World %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageLocalToWorld));
	printf("  -> Backface Culling %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageBackfaceCulling));
	printf("  -> Rasterization %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization));
	printf("     -> PixelDraw %lldmics \n", PerformanceCounter_GetAverage(&p_wireframeRenderPerformance->AverageRasterization_PixelDrawing));
};
#endif