#include "SolidRenderer.h"

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
#include "Renderer/PixelColor/PixelColorCalculation.h"
#include "Metrics/RenderTimeMetrics.h"
#include "Algorithm/Sort_alg.h"

ARRAY_ALLOC_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)

ARRAY_ALLOC_FUNCTION(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)

ARRAY_ALLOC_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)
ARRAY_RESIZE_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)

SORT_QUICK_ALGORITHM(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed,
	SQA_ComparedElementValueExpression.DistanceFromCamera > l_pivot->DistanceFromCamera,
	SQA_ComparedElementValueExpressionInvert.DistanceFromCamera < l_pivot->DistanceFromCamera);

ARRAY_ALLOC_FUNCTION(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)

// There is a unique directional light
RenderLights GRenderLights =
{
	.DirectionalLight = {.Direction = {1.0f, 0.0f, 0.0f}, .Intensity = 1.0f, .Color = {1.0f, 1.0f, 1.0f} },
	.AmbientLight = {.Color = {0.1f, 0.1f, 0.1f} }
};

inline void WireframeRenderer_CalculatePixelPosition_FromWorldPosition(VertexPipeline_PTR p_vertex, const SolidRendererInput* p_input);

//Paint algorithm -> sorting polygons
// Sorts polygon based on z coordinates of camera projected vertices
inline void SolidRenderer_SortPolygonsForRendering(SolidRenderer_Memory_PTR p_solidRendererMemory);

void SolidRenderer_renderV2(const SolidRendererInput* p_input, Texture3f_PTR p_to, Recti_PTR p_to_clipRect, SolidRenderer_Memory* p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	TimeClockPrecision l_wireframeRenderBegin = Clock_currentTime_mics();
#endif

	SolidRenderer_Memory_clear(p_memory, p_to->Width, p_to->Height);
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
#if RENDER_PERFORMANCE_TIMER
			tmp_timer_2 = Clock_currentTime_mics();
#endif

			size_t l_vertexIndexOffset = p_memory->VertexPipeline.Size;
			for (size_t j = 0; j < l_renderableObject->Mesh->Vertices.Size; j++)
			{
				Arr_PushBackRealloc_Empty_VertexPipeline(&p_memory->VertexPipeline);
				p_memory->VertexPipeline.Memory[p_memory->VertexPipeline.Size - 1] = (VertexPipeline){
					.Vertex = l_renderableObject->Mesh->Vertices.Memory[j],
					.CameraPositionCalculated = 0,
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
										.Material = l_renderableObject->Material
				};

				if (l_renderableObjectMaterial->MeshPropertyUsage == MATERIAL_MESHPROPERTY_USAGE_UV)
				{
					l_polygonPipeline->MaterialMeshProperties.PolygonUV = l_renderableObject->Mesh->PerVertexData.UV1.Memory[j];
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
			.v1 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesPipelineIndex.v1].TransformedPosition,
			.v2 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesPipelineIndex.v2].TransformedPosition,
			.v3 = &p_memory->VertexPipeline.Memory[l_polygon->VerticesPipelineIndex.v3].TransformedPosition
		};

		Vector4f l_worldNormal;
		Polygon_CalculateNormal_V4FPTR(&l_poly, &l_worldNormal);

		l_polygon->IsCulled = BackFaceCulled_Normal3fPTR(&l_worldNormal, l_poly.v1, &p_input->CameraBuffer->WorldPosition);

		if (!l_polygon->IsCulled)
		{
			PixelColorCaluclation_Polygon_PushCalculations(l_polygon, &l_worldNormal, p_memory);
		}
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AverageBackfaceCulling, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	// Light Calculations
	for (size_t i = 0; i < p_memory->FlatShadingCalculations.Size; i++)
	{
		FlatShadingPixelCalculation_PreCalculation(&p_memory->FlatShadingCalculations.Memory[i], &GRenderLights, p_memory);
	}

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AveragePreRasterizationLightCalculation, Clock_currentTime_mics() - tmp_timer);
#endif


	//World to camera
	for (size_t i = 0; i < p_memory->PolygonPipelines.Size; i++)
	{
		PolygonPipelineV2_PTR l_polygonPipeline = &p_memory->PolygonPipelines.Memory[i];

		if (!l_polygonPipeline->IsCulled)
		{
			VertexPipeline_PTR l_v1 = &p_memory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v1];
			VertexPipeline_PTR l_v2 = &p_memory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v2];
			VertexPipeline_PTR l_v3 = &p_memory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v3];

			if (!l_v1->CameraPositionCalculated) { Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &l_v1->TransformedPosition, &l_v1->CameraSpacePosition); l_v1->CameraPositionCalculated = 1; }
			if (!l_v2->CameraPositionCalculated) { Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &l_v2->TransformedPosition, &l_v2->CameraSpacePosition); l_v2->CameraPositionCalculated = 1; }
			if (!l_v3->CameraPositionCalculated) { Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &l_v3->TransformedPosition, &l_v3->CameraSpacePosition); l_v3->CameraPositionCalculated = 1; }

		}
	}

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	SolidRenderer_SortPolygonsForRendering(p_memory);

#if RENDER_PERFORMANCE_TIMER
	PerformanceCounter_PushSample(&GWireframeRendererPerformace.AveragePolygonSorting, Clock_currentTime_mics() - tmp_timer);
#endif

#if RENDER_PERFORMANCE_TIMER
	tmp_timer = Clock_currentTime_mics();
#endif

	for (size_t i = 0; i < p_memory->OrderedPolygonPipelinesIndex.Size; i++)
	{
		PolygonPipelineV2_PTR l_polygonPipeline = &p_memory->PolygonPipelines.Memory[p_memory->OrderedPolygonPipelinesIndex.Memory[i].Index];

#if RENDER_PERFORMANCE_TIMER
		tmp_timer_2 = Clock_currentTime_mics();
#endif

		VertexPipeline_PTR l_v1 = &p_memory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v1];
		VertexPipeline_PTR l_v2 = &p_memory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v2];
		VertexPipeline_PTR l_v3 = &p_memory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v3];

		WireframeRenderer_CalculatePixelPosition_FromWorldPosition(l_v1, p_input);
		WireframeRenderer_CalculatePixelPosition_FromWorldPosition(l_v2, p_input);
		WireframeRenderer_CalculatePixelPosition_FromWorldPosition(l_v3, p_input);

#if RENDER_PERFORMANCE_TIMER
		PerformanceCounter_AddTime(&GWireframeRendererPerformace.AverageRasterization_TransformCoords, Clock_currentTime_mics() - tmp_timer_2);
#endif

		Polygon2i l_polygon = {
			.v1 = l_v1->PixelPosition,
			.v2 = l_v2->PixelPosition,
			.v3 = l_v3->PixelPosition
		};

		Draw_PolygonClipped(l_polygonPipeline, &l_polygon, p_to, p_to_clipRect, &GRenderLights, p_memory);
	}


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


inline void WireframeRenderer_CalculatePixelPosition_FromWorldPosition(VertexPipeline_PTR p_vertex, const SolidRendererInput* p_input)
{
	if (!p_vertex->PixelPositionCalculated)
	{
		// Camera to clip
		Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &p_vertex->CameraSpacePosition, &p_vertex->TransformedPosition);

		// To pixel
		WindowSize_GraphicsAPIToPixel(&p_input->WindowSize, p_vertex->TransformedPosition.x, p_vertex->TransformedPosition.y, &p_vertex->PixelPosition.x, &p_vertex->PixelPosition.y);

		p_vertex->PixelPositionCalculated = 1;
	}
};

inline void SolidRenderer_SortPolygonsForRendering(SolidRenderer_Memory_PTR p_solidRendererMemory)
{
	for (size_t i = 0; i < p_solidRendererMemory->PolygonPipelines.Size; i++)
	{
		PolygonPipelineV2_PTR l_polygonPipeline = &p_solidRendererMemory->PolygonPipelines.Memory[i];

		if (!l_polygonPipeline->IsCulled)
		{
			Arr_PushBackRealloc_Empty_PolygonPipeline_CameraDistanceIndexed(&p_solidRendererMemory->OrderedPolygonPipelinesIndex);
			p_solidRendererMemory->OrderedPolygonPipelinesIndex.Memory[p_solidRendererMemory->OrderedPolygonPipelinesIndex.Size - 1] = (PolygonPipeline_CameraDistanceIndexed)
			{
				.Index = i,
				.DistanceFromCamera = ((p_solidRendererMemory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v1].CameraSpacePosition.z +
						p_solidRendererMemory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v2].CameraSpacePosition.z +
						p_solidRendererMemory->VertexPipeline.Memory[l_polygonPipeline->VerticesPipelineIndex.v3].CameraSpacePosition.z) * 0.333333f)
			};
		}
	}
	Sort_Quick_PolygonPipeline_CameraDistanceIndexed(&p_solidRendererMemory->OrderedPolygonPipelinesIndex);
};


void SolidRenderer_Memory_alloc(SolidRenderer_Memory* p_memory)
{
	Arr_Alloc_RenderableObjectPipeline(&p_memory->RederableObjectsPipeline, 0);
	Arr_Alloc_VertexPipeline(&p_memory->VertexPipeline, 0);
	Arr_Alloc_PolygonPipelineV2(&p_memory->PolygonPipelines, 0);
	Arr_Alloc_PolygonPipeline_CameraDistanceIndexed(&p_memory->OrderedPolygonPipelinesIndex, 0);
	Arr_Alloc_FlatShadingPixelCalculation(&p_memory->FlatShadingCalculations, 0);
};
void SolidRenderer_Memory_clear(SolidRenderer_Memory* p_memory, size_t p_width, size_t height)
{
	Arr_Clear(&p_memory->RederableObjectsPipeline.array);
	Arr_Clear(&p_memory->VertexPipeline.array);
	Arr_Clear(&p_memory->PolygonPipelines.array);
	Arr_Clear(&p_memory->OrderedPolygonPipelinesIndex.array);
	Arr_Clear(&p_memory->FlatShadingCalculations.array);
};
void SolidRenderer_Memory_free(SolidRenderer_Memory* p_memory)
{
#if RENDER_PERFORMANCE_TIMER
	SolidRendererMetrics_Print(&GWireframeRendererPerformace);
#endif

	Arr_Free(&p_memory->RederableObjectsPipeline.array);
	Arr_Free(&p_memory->VertexPipeline.array);
	Arr_Free(&p_memory->PolygonPipelines.array);
	Arr_Free(&p_memory->OrderedPolygonPipelinesIndex.array);
	Arr_Free(&p_memory->FlatShadingCalculations.array);
};