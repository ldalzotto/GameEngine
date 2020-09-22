#include "RenderableObjectTransform.h"
#include "v2/_interface/MatrixC.h"
#include "Objects/Resource/Polygon.h"
#include "Cull/BackfaceCulling.h"

inline void _i_VertexPipeline_CalculateWorldPosition(VertexPipeline_PTR p_vertexPipeline, RenderableObjectPipeline_PTR p_renderableObject, RenderableObjectTransform_Input_PTR p_input)
{
	if (!p_vertexPipeline->WorldPositionCalculated)
	{
		Vertex_PTR l_vertex = &p_input->RenderHeap->VertexAllocator.array.Memory[p_vertexPipeline->Vertex.Handle];
		Mat_Mul_M4F_V4F(&p_renderableObject->RenderedObject->ModelMatrix, &l_vertex->LocalPosition, &p_vertexPipeline->WorldPosition);
		p_vertexPipeline->WorldPositionCalculated = 1;
	}
};

inline void _i_VertexPipeline_CalculatePixelPosition(VertexPipeline_PTR p_vertexPipeline, RenderableObjectTransform_Input_PTR p_input)
{
	if (!p_vertexPipeline->PixelPositionCalculated)
	{
		// World to Camera
		Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &p_vertexPipeline->WorldPosition, &p_vertexPipeline->CameraSpacePosition);

		// Camera to clip
		Vector4f l_clipPosition;
		Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &p_vertexPipeline->CameraSpacePosition, &l_clipPosition);

		// To pixel
		WindowSize_GraphicsAPIToPixel(p_input->WindowSize, l_clipPosition.x, l_clipPosition.y, &p_vertexPipeline->PixelPosition.x, &p_vertexPipeline->PixelPosition.y);

		p_vertexPipeline->PixelPositionCalculated = 1;
	}
};

void RendereableObject_TransformPolygons(RenderableObjectTransform_Input_PTR p_input)
{
	for (size_t i = 0; i < p_input->RendererPipelineMemory->PolygonPipelines.Size; i++)
	{
		PolygonPipelineV2_PTR l_polygon = &p_input->RendererPipelineMemory->PolygonPipelines.Memory[i];
		RenderableObjectPipeline_PTR l_renderableObject = &p_input->RendererPipelineMemory->RederableObjectsPipeline.Memory[l_polygon->AssociatedRenderableObjectPipeline];

		VertexPipeline_PTR l_v1 = &p_input->RendererPipelineMemory->VertexPipeline.Memory[l_polygon->VerticesPipelineIndex.v1];
		VertexPipeline_PTR l_v2 = &p_input->RendererPipelineMemory->VertexPipeline.Memory[l_polygon->VerticesPipelineIndex.v2];
		VertexPipeline_PTR l_v3 = &p_input->RendererPipelineMemory->VertexPipeline.Memory[l_polygon->VerticesPipelineIndex.v3];

		// Local to world
		_i_VertexPipeline_CalculateWorldPosition(l_v1, l_renderableObject, p_input);
		_i_VertexPipeline_CalculateWorldPosition(l_v2, l_renderableObject, p_input);
		_i_VertexPipeline_CalculateWorldPosition(l_v3, l_renderableObject, p_input);

		// Backface culling
		Polygon4fPTR l_worldPositionPoly =
		{
			.v1 = &l_v1->WorldPosition,
			.v2 = &l_v2->WorldPosition,
			.v3 = &l_v3->WorldPosition
		};

		Polygon_CalculateNormal_V3FPTR(&l_worldPositionPoly, &l_polygon->CalculatedFlatWorldNormal);
		l_polygon->IsCulled = BackFaceCulled_Normal3fPTR(&l_polygon->CalculatedFlatWorldNormal, l_worldPositionPoly.v1, &p_input->CameraBuffer->WorldPosition);

		if (!l_polygon->IsCulled)
		{
			// World to camera to clip to pixel
			_i_VertexPipeline_CalculatePixelPosition(l_v1, p_input);
			_i_VertexPipeline_CalculatePixelPosition(l_v2, p_input);
			_i_VertexPipeline_CalculatePixelPosition(l_v3, p_input);
		}

	}
};