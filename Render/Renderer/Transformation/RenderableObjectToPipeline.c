#include "RenderableObjectToPipeline.h"

#include "DataStructures/ARRAY.h"

ARRAY_RESIZE_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(RenderableObjectPipeline, Array_RenderableObjectPipeline_PTR, RenderableObjectPipeline)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(PolygonPipelineV2, ARRAY_PolygonPipelineV2_PTR, PolygonPipelineV2)
ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(VertexPipeline, Array_VertexPipeline_PTR, VertexPipeline)

void RenderableObject_ToRenderPipeline(RENDEREDOBJECT_BUFFER_PTR p_renderableObjects, RendererPipeline_Memory_PTR p_renderPipelineMemory,  RenderHeap_PTR p_renderHeap)
{
	for (size_t i = 0; i < p_renderableObjects->RenderedObjects.Size; i++)
	{
		RenderedObject_HANDLE l_renderableObjectHandle = p_renderableObjects->RenderedObjects.Memory[i];
		RenderedObject_PTR l_renderableObject = &p_renderHeap->RenderedObjectAllocator.array.Memory[l_renderableObjectHandle.Handle];
		if (!l_renderableObject->IsCulled)
		{

			size_t l_vertexIndexOffset = p_renderPipelineMemory->VertexPipeline.Size;
			for (size_t j = 0; j < l_renderableObject->Mesh->Vertices.Size; j++)
			{
				Arr_PushBackRealloc_Empty_VertexPipeline(&p_renderPipelineMemory->VertexPipeline);
				p_renderPipelineMemory->VertexPipeline.Memory[p_renderPipelineMemory->VertexPipeline.Size - 1] = (VertexPipeline){
					.Vertex = l_renderableObject->Mesh->Vertices.Memory[j],
					.AssociatedRenderableObjectPipeline = p_renderPipelineMemory->RederableObjectsPipeline.Size,
					.WorldPositionCalculated = 0,
					.PixelPositionCalculated = 0
				};
			}

			size_t l_polygonPipelineIndexOffset = p_renderPipelineMemory->PolygonPipelines.Size;

			Material* l_renderableObjectMaterial = &p_renderHeap->MaterialAllocator.array.Memory[l_renderableObject->Material.Handle];

			for (size_t j = 0; j < l_renderableObject->Mesh->Polygons.Size; j++)
			{
				Polygon_VertexIndex_PTR l_polygon = &p_renderHeap->PolygonAllocator.array.Memory[l_renderableObject->Mesh->Polygons.Memory[j].Handle];

				Arr_PushBackRealloc_Empty_PolygonPipelineV2(&p_renderPipelineMemory->PolygonPipelines);
				PolygonPipelineV2* l_polygonPipeline = &p_renderPipelineMemory->PolygonPipelines.Memory[p_renderPipelineMemory->PolygonPipelines.Size - 1];
				*l_polygonPipeline = (PolygonPipelineV2){
										.IsCulled = 0,
										.VerticesPipelineIndex = {
											l_polygon->v1 + l_vertexIndexOffset,
											l_polygon->v2 + l_vertexIndexOffset,
											l_polygon->v3 + l_vertexIndexOffset
										},
										.AssociatedRenderableObjectPipeline = p_renderPipelineMemory->RederableObjectsPipeline.Size,
										.Material = l_renderableObject->Material,
										.MeshPolygonIndex = j
				};
			}

			Arr_PushBackRealloc_Empty_RenderableObjectPipeline(&p_renderPipelineMemory->RederableObjectsPipeline);
			p_renderPipelineMemory->RederableObjectsPipeline.Memory[p_renderPipelineMemory->RederableObjectsPipeline.Size - 1] = (RenderableObjectPipeline){
				.RenderedObject = l_renderableObjectHandle,
				.PolygonPipelineIndexBeginIncluded = l_polygonPipelineIndexOffset,
				.PolygonPipelineIndexEndExcluded = p_renderPipelineMemory->PolygonPipelines.Size,
				.VertexPipelineIndexBeginIncluded = l_vertexIndexOffset,
				.VertexPipelineIndexEndExcluded = p_renderPipelineMemory->VertexPipeline.Size
			};

		}
	}
};