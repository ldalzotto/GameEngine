#include "Mesh.h"

#include "Utils/Algorithm/Algorithm.h"
#include "Render/LoopStep/PreRenderStagingStep.h"

namespace _GameEngine::_Render
{
	/**
		Called when the @ref PreRenderStagging step has been completed.
	*/
	void meshOnPreRenderStagingCompleted(void* p_mesh)
	{
		Mesh* l_mesh = (Mesh*)p_mesh;
		l_mesh->PreRenderStagingCompleted = true;
	};

	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo)
	{
		p_mesh->PreRenderStagingCompleted = false;
		p_mesh->PreRenderStagging = p_meshAllocInfo->PreRenderStagging;

		{
			BufferAllocInfo l_vertexBufferAllocInfo{};
			l_vertexBufferAllocInfo.Size = sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
			l_vertexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			l_vertexBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			VulkanBuffer_alloc(&p_mesh->VertexBuffer, &l_vertexBufferAllocInfo, p_meshAllocInfo->Device);

			VulkanBuffer l_vertexStagingBuffer{};
			BufferAllocInfo l_vertexStaggingBufferAllocInfo{};
			l_vertexStaggingBufferAllocInfo.Size = sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
			l_vertexStaggingBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			l_vertexStaggingBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			VulkanBuffer_alloc(&l_vertexStagingBuffer, &l_vertexStaggingBufferAllocInfo, p_meshAllocInfo->Device);
			VulkanBuffer_pushToGPU(&l_vertexStagingBuffer, p_meshAllocInfo->Device, p_mesh->Vertices.data(), static_cast<size_t>(l_vertexStagingBuffer.BufferAllocInfo.Size));

			PreRenderStaggingOperation l_vertexStagingPushToGPU{};
			l_vertexStagingPushToGPU.StagingBuffer = l_vertexStagingBuffer;
			l_vertexStagingPushToGPU.TargetBuffer = &p_mesh->VertexBuffer;
			l_vertexStagingPushToGPU.StaggingOperationCancelled = false;
			l_vertexStagingPushToGPU.PrerenderStaggingOperationCompleted.Closure = p_mesh;
			l_vertexStagingPushToGPU.PrerenderStaggingOperationCompleted.OnStaggingDone = meshOnPreRenderStagingCompleted;

			p_meshAllocInfo->PreRenderStagging->StaggingOperations.emplace_back(l_vertexStagingPushToGPU);
		}

		{
			BufferAllocInfo l_indexBufferAllocInfo{};
			l_indexBufferAllocInfo.Size = sizeof(p_mesh->Indices[0]) * p_mesh->Indices.size();
			l_indexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			l_indexBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			VulkanBuffer_alloc(&p_mesh->IndicesBuffer, &l_indexBufferAllocInfo, p_meshAllocInfo->Device);

			VulkanBuffer l_indexStaggingBuffer{};
			BufferAllocInfo l_indexBufferStagingAllocInfo{};
			l_indexBufferStagingAllocInfo.Size = sizeof(p_mesh->Indices[0]) * p_mesh->Indices.size();
			l_indexBufferStagingAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			l_indexBufferStagingAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			VulkanBuffer_alloc(&l_indexStaggingBuffer, &l_indexBufferStagingAllocInfo, p_meshAllocInfo->Device);
			VulkanBuffer_pushToGPU(&l_indexStaggingBuffer, p_meshAllocInfo->Device, p_mesh->Indices.data(), static_cast<size_t>(l_indexStaggingBuffer.BufferAllocInfo.Size));

			PreRenderStaggingOperation l_indexStagingPushToGPU{};
			l_indexStagingPushToGPU.StagingBuffer = l_indexStaggingBuffer;
			l_indexStagingPushToGPU.TargetBuffer = &p_mesh->IndicesBuffer;
			l_indexStagingPushToGPU.StaggingOperationCancelled = false;
			l_indexStagingPushToGPU.PrerenderStaggingOperationCompleted.Closure = p_mesh;
			l_indexStagingPushToGPU.PrerenderStaggingOperationCompleted.OnStaggingDone = meshOnPreRenderStagingCompleted;

			p_meshAllocInfo->PreRenderStagging->StaggingOperations.emplace_back(l_indexStagingPushToGPU);
		}

	};

	void Mesh_free(Mesh* p_mesh, Device* p_device)
	{
		// If the pre render staging is not completed, this means that the mesh has been created and destroyed within a single update frame before the render stap has been called
		if (!p_mesh->PreRenderStagingCompleted)
		{
			for (PreRenderStaggingOperation& l_preRenderStaggingOpertion : p_mesh->PreRenderStagging->StaggingOperations)
			{
				if (l_preRenderStaggingOpertion.TargetBuffer == &p_mesh->IndicesBuffer
					|| l_preRenderStaggingOpertion.TargetBuffer == &p_mesh->VertexBuffer)
				{
					l_preRenderStaggingOpertion.StaggingOperationCancelled = true;
				}
			}
		}

		VulkanBuffer_free(&p_mesh->VertexBuffer, p_device);
		VulkanBuffer_free(&p_mesh->IndicesBuffer, p_device);
	};
}


