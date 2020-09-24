#include "RenderableObjectTransform.h"

#include "DataStructures/ARRAY.h"
#include "v2/_interface/MatrixC.h"
#include "Objects/Resource/Polygon.h"
#include "Cull/BackfaceCulling.h"
#include "Algorithm/Sort_alg.h"

ARRAY_PUSHBACKREALLOC_ENPTY_FUNCTION(PolygonPipeline_CameraDistanceIndexed, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed)
SORT_QUICK_ALGORITHM(PolygonPipeline_CameraDistanceIndexed_FrontToFar, ARRAY_PolygonPipeline_CameraDistanceIndexed_PTR, PolygonPipeline_CameraDistanceIndexed,
	SQA_ComparedElementValueExpression.DistanceFromCamera < l_pivot->DistanceFromCamera,
	SQA_ComparedElementValueExpressionInvert.DistanceFromCamera > l_pivot->DistanceFromCamera);

inline void _i_VertexPipeline_CalculateWorldPosition(VertexPipeline_PTR p_vertexPipeline, RenderedObject_PTR p_renderableObject, RenderableObjectTransform_Input_PTR p_input)
{
	if (!p_vertexPipeline->WorldPositionCalculated)
	{
		Vertex_PTR l_vertex = &p_input->RenderHeap->VertexAllocator.array.Memory[p_vertexPipeline->Vertex.Handle];
		Mat_Mul_M4F_V4F(&p_renderableObject->ModelMatrix, &l_vertex->LocalPosition, &p_vertexPipeline->WorldPosition);
		p_vertexPipeline->WorldPositionCalculated = 1;
	}
};

inline void _i_VertexPipeline_CalculatePixelPosition(VertexPipeline_PTR p_vertexPipeline, RenderableObjectTransform_Input_PTR p_input)
{
	if (!p_vertexPipeline->PixelPositionCalculated)
	{
		// World to Camera
		Mat_Mul_M4F_V4F(&p_input->CameraBuffer->ViewMatrix, &p_vertexPipeline->WorldPosition, &p_vertexPipeline->CameraSpacePosition);

		// Camera to clip
		Vector4f l_clipPosition;
		Mat_Mul_M4F_V4F_Homogeneous(&p_input->CameraBuffer->ProjectionMatrix, &p_vertexPipeline->CameraSpacePosition, &l_clipPosition);

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
		RenderedObject_PTR l_renderableObject =
			&p_input->RenderHeap->RenderedObjectAllocator.array.Memory[p_input->RendererPipelineMemory->RederableObjectsPipeline.Memory[l_polygon->AssociatedRenderableObjectPipeline].RenderedObject.Handle];

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

			// Push polygon to the indexed list
			Arr_PushBackRealloc_Empty_PolygonPipeline_CameraDistanceIndexed(&p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex);
			p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex.Memory[p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex.Size - 1] = (PolygonPipeline_CameraDistanceIndexed)
			{
				.Index = i,
				.DistanceFromCamera = ((l_v1->CameraSpacePosition.z + l_v2->CameraSpacePosition.z + l_v3->CameraSpacePosition.z) * 0.333333f)
			};
		}

	}

	// Polygon sorting is done so that the nearest polygon from camera is rendered first.
	// This is to have better chance to discard pixel draw calculation (thanks to the depth buffer)
	Sort_Quick_PolygonPipeline_CameraDistanceIndexed_FrontToFar(&p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex);
};

#if HAS_OPENCL
#include <CL/cl.h>
#include "Renderer/OpenCL/OpenCLContext.h"
#include "Clock/Clock.h"
#include "Renderer/Pipeline/RendererPipelineMemory.h"

void RendereableObject_TransformPolygons_vertexCentralized_GPU(RenderableObjectTransform_Input_PTR p_input, OpenCLContext_PTR p_openCLContext)
{
	Arr_Resize_PolygonPipeline_CameraDistanceIndexed(&p_input->RendererPipelineMemory->PolygonPipelinesIndexNotFiltered, p_input->RendererPipelineMemory->PolygonPipelines.Size);
	p_input->RendererPipelineMemory->PolygonPipelinesIndexNotFiltered.Size = p_input->RendererPipelineMemory->PolygonPipelines.Size;



	cl_mem l_vertexPipelineBuf = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(VertexPipeline) * p_input->RendererPipelineMemory->VertexPipeline.Size, p_input->RendererPipelineMemory->VertexPipeline.Memory, NULL);
	cl_mem l_polygonPipelineBuf = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(PolygonPipelineV2) * p_input->RendererPipelineMemory->PolygonPipelines.Size, p_input->RendererPipelineMemory->PolygonPipelines.Memory, NULL);
	cl_mem l_renderableObjectPipelineBuf = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(RenderableObjectPipeline) * p_input->RendererPipelineMemory->RederableObjectsPipeline.Size, p_input->RendererPipelineMemory->RederableObjectsPipeline.Memory, NULL);

	cl_mem l_polygonIndexNotSortedBuf = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(PolygonPipeline_CameraDistanceIndexed) * p_input->RendererPipelineMemory->PolygonPipelinesIndexNotFiltered.Size, p_input->RendererPipelineMemory->PolygonPipelinesIndexNotFiltered.Memory, NULL);

	cl_mem l_vertexBuf = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(Vertex) * p_input->RenderHeap->VertexAllocator.array.Size, p_input->RenderHeap->VertexAllocator.array.Memory, NULL);
	cl_mem l_renderedObjectBuf = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(RenderedObject) * p_input->RenderHeap->RenderedObjectAllocator.array.Size, p_input->RenderHeap->RenderedObjectAllocator.array.Memory, NULL);

	cl_mem l_cameraBuffer = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(CAMERABUFFER), p_input->CameraBuffer, NULL);
	cl_mem l_windowSizeBuffer = clCreateBuffer(p_openCLContext->Context, CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(WindowSize), p_input->WindowSize, NULL);

	
	//__kernel Draw_Transform(__global VertexPipeline* p_vertexPipelines, __global RenderableObjectPipeline* p_renderableObjectsPipeline, __global Vertex* p_vertices, __global RenderedObject* p_renderedObjects)

	clSetKernelArg(TransformKernel, 0, sizeof(cl_mem), (void*)&l_vertexPipelineBuf);
	clSetKernelArg(TransformKernel, 1, sizeof(cl_mem), (void*)&l_renderableObjectPipelineBuf);
	clSetKernelArg(TransformKernel, 2, sizeof(cl_mem), (void*)&l_vertexBuf);
	clSetKernelArg(TransformKernel, 3, sizeof(cl_mem), (void*)&l_renderedObjectBuf);
	clSetKernelArg(TransformKernel, 4, sizeof(cl_mem), (void*)&l_cameraBuffer);
	clSetKernelArg(TransformKernel, 5, sizeof(cl_mem), (void*)&l_windowSizeBuffer);

	size_t global_work_size_transform[1] = { p_input->RendererPipelineMemory->VertexPipeline.Size };
	clEnqueueNDRangeKernel(p_openCLContext->CommandQueue, TransformKernel, 1, NULL, global_work_size_transform, NULL, 0, NULL, NULL);
	
	clSetKernelArg(PolygonNormalAndBackfaceCullKernel, 0, sizeof(cl_mem), (void*)&l_polygonPipelineBuf);
	clSetKernelArg(PolygonNormalAndBackfaceCullKernel, 1, sizeof(cl_mem), (void*)&l_vertexPipelineBuf);
	clSetKernelArg(PolygonNormalAndBackfaceCullKernel, 2, sizeof(cl_mem), (void*)&l_cameraBuffer);
	clSetKernelArg(PolygonNormalAndBackfaceCullKernel, 3, sizeof(cl_mem), (void*)&l_polygonIndexNotSortedBuf);

	size_t global_work_size_backCull[1] = { p_input->RendererPipelineMemory->PolygonPipelines.Size };
	clEnqueueNDRangeKernel(p_openCLContext->CommandQueue, PolygonNormalAndBackfaceCullKernel, 1, NULL, global_work_size_backCull, NULL, 0, NULL, NULL);
	// l_error = clEnQueuN

	clEnqueueWriteBuffer(p_openCLContext->CommandQueue, &l_vertexPipelineBuf, CL_TRUE, 0, sizeof(VertexPipeline) * p_input->RendererPipelineMemory->VertexPipeline.Size, p_input->RendererPipelineMemory->VertexPipeline.Memory, 0, NULL, NULL);
	clEnqueueWriteBuffer(p_openCLContext->CommandQueue, &l_vertexPipelineBuf, CL_TRUE, 0, sizeof(PolygonPipelineV2) * p_input->RendererPipelineMemory->PolygonPipelines.Size, p_input->RendererPipelineMemory->PolygonPipelines.Memory, 0, NULL, NULL);
	clEnqueueWriteBuffer(p_openCLContext->CommandQueue, &l_vertexPipelineBuf, CL_TRUE, 0, sizeof(PolygonPipeline_CameraDistanceIndexed) * p_input->RendererPipelineMemory->PolygonPipelinesIndexNotFiltered.Size, p_input->RendererPipelineMemory->PolygonPipelinesIndexNotFiltered.Memory, 0, NULL, NULL);

	clFinish(p_openCLContext->CommandQueue);

	

	clReleaseMemObject(l_vertexPipelineBuf);
	clReleaseMemObject(l_renderableObjectPipelineBuf);
	clReleaseMemObject(l_vertexBuf);
	clReleaseMemObject(l_polygonIndexNotSortedBuf);
	clReleaseMemObject(l_renderedObjectBuf);
	clReleaseMemObject(l_cameraBuffer);
	clReleaseMemObject(l_windowSizeBuffer);
	
	size_t l_polygonIndex = 0;
	Arr_Resize_PolygonPipeline_CameraDistanceIndexed(&p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex, p_input->RendererPipelineMemory->PolygonPipelines.Size);

	for (size_t i = 0; i < p_input->RendererPipelineMemory->PolygonPipelines.Size; i++)
	{
		PolygonPipelineV2_PTR l_polygon = &p_input->RendererPipelineMemory->PolygonPipelines.Memory[i];

		if (!l_polygon->IsCulled)
		{
			p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex.Memory[l_polygonIndex] = p_input->RendererPipelineMemory->PolygonPipelinesIndexNotFiltered.Memory[i];
			l_polygonIndex += 1;
		}
	}

	p_input->RendererPipelineMemory->OrderedPolygonPipelinesIndex.Size = l_polygonIndex;
}

#endif