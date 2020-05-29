#include "Mesh.h"

#include "Render/LoopStep/PreRenderStagingStep.h"

namespace _GameEngine::_Render
{
	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo)
	{
		p_mesh->Vertices = *p_meshAllocInfo->Vertices;
		p_mesh->Indices = *p_meshAllocInfo->Indices;

		BufferAllocInfo l_vertexBufferAllocInfo{};
		l_vertexBufferAllocInfo.Size = sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
		l_vertexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		l_vertexBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		VulkanBuffer_alloc(&p_mesh->VertexBuffer, &l_vertexBufferAllocInfo, p_meshAllocInfo->Device);

		VulkanBuffer l_vertexStagingBuffer{};
		BufferAllocInfo l_vertexStaggingBufferAllocInfo{};
		l_vertexStaggingBufferAllocInfo.Size= sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
		l_vertexStaggingBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		l_vertexStaggingBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		VulkanBuffer_alloc(&l_vertexStagingBuffer, &l_vertexStaggingBufferAllocInfo, p_meshAllocInfo->Device);
		VulkanBuffer_pushToGPU(&l_vertexStagingBuffer, p_meshAllocInfo->Device, p_mesh->Vertices.data(), static_cast<size_t>(l_vertexStagingBuffer.BufferAllocInfo.Size));
		PreRenderStaggingOperation l_vertexStagingPushToGPU{};
		l_vertexStagingPushToGPU.StagingBuffer = l_vertexStagingBuffer;
		l_vertexStagingPushToGPU.TargetBuffer = &p_mesh->VertexBuffer;
		p_meshAllocInfo->PreRenderStagging->StaggingOprtations.push_back(l_vertexStagingPushToGPU);

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
		p_meshAllocInfo->PreRenderStagging->StaggingOprtations.push_back(l_indexStagingPushToGPU);
	};

	void Mesh_free(Mesh* p_mesh, Device* p_device)
	{
		VulkanBuffer_free(&p_mesh->VertexBuffer, p_device);
		VulkanBuffer_free(&p_mesh->IndicesBuffer, p_device);
	};
}


