#include "Mesh.h"

#include "Utils/Algorithm/Algorithm.h"

#include "Render/LoopStep/PreRenderDeferedCommandBufferStep.h"
#include "Render/CommandBuffer/DeferredOperations/BufferCopyDeferredOperation.h"

namespace _GameEngine::_Render
{
	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo)
	{
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

			BufferCopyOperation* l_bufferCopyOperation = new BufferCopyOperation();
			l_bufferCopyOperation->Device = p_meshAllocInfo->Device;
			l_bufferCopyOperation->SourceBuffer = l_vertexStagingBuffer;
			l_bufferCopyOperation->TargetBuffer = &p_mesh->VertexBuffer;
			DeferredCommandBufferOperation l_vertexStagingCopyOperation = BufferCopyDeferredOperation_build(&l_bufferCopyOperation);
			p_mesh->VerticesStagingBufferCompletionToken = l_vertexStagingCopyOperation.DeferredCommandBufferCompletionToken;
			p_meshAllocInfo->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_vertexStagingCopyOperation));

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

			BufferCopyOperation* l_bufferCopyOperation = new BufferCopyOperation();
			l_bufferCopyOperation->Device = p_meshAllocInfo->Device;
			l_bufferCopyOperation->SourceBuffer = l_indexStaggingBuffer;
			l_bufferCopyOperation->TargetBuffer = &p_mesh->IndicesBuffer;
			p_mesh->IndicesStagingBufferCompletionToken = new DeferredCommandBufferCompletionToken();
			DeferredCommandBufferOperation l_indexStagingCopyOperation = BufferCopyDeferredOperation_build(&l_bufferCopyOperation);
			p_mesh->IndicesStagingBufferCompletionToken = l_indexStagingCopyOperation.DeferredCommandBufferCompletionToken;
			p_meshAllocInfo->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_indexStagingCopyOperation));
		}

	};

	void Mesh_free(Mesh* p_mesh, Device* p_device)
	{
		p_mesh->IndicesStagingBufferCompletionToken->IsCancelled = true;
		p_mesh->VerticesStagingBufferCompletionToken->IsCancelled = true;
		VulkanBuffer_free(&p_mesh->VertexBuffer, p_device);
		VulkanBuffer_free(&p_mesh->IndicesBuffer, p_device);
	};
}


