#include "Mesh.h"

namespace _GameEngine::_Render::_Mesh
{
	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo)
	{
		p_mesh->Vertices = *p_meshAllocInfo->Vertices;
		p_mesh->Indices = *p_meshAllocInfo->Indices;

		_Memory::BufferAllocInfo l_vertexBufferAllocInfo{};
		l_vertexBufferAllocInfo.Size = sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
		l_vertexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		l_vertexBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		VulkanBuffer_alloc(&p_mesh->VertexBuffer, &l_vertexBufferAllocInfo, p_meshAllocInfo->Device);

		_Memory::BufferAllocInfo l_vertexStaggingBufferAllocInfo{};
		l_vertexStaggingBufferAllocInfo.Size= sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
		l_vertexStaggingBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		l_vertexStaggingBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		VulkanBuffer_alloc(&p_mesh->VertexStaggingBuffer, &l_vertexStaggingBufferAllocInfo, p_meshAllocInfo->Device);

		_Memory::VulkanBuffer_pushToGPU(&p_mesh->VertexStaggingBuffer, p_meshAllocInfo->Device, p_mesh->Vertices.data(), static_cast<size_t>(p_mesh->VertexStaggingBuffer.BufferCreateInfo.size));

		_Memory::BufferAllocInfo l_indexBufferAllocInfo{};
		l_indexBufferAllocInfo.Size = sizeof(p_mesh->Indices[0]) * p_mesh->Indices.size();
		l_indexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		l_indexBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		VulkanBuffer_alloc(&p_mesh->IndicesBuffer, &l_indexBufferAllocInfo, p_meshAllocInfo->Device);
		
		_Memory::BufferAllocInfo l_indexBufferStagingAllocInfo{};
		l_indexBufferStagingAllocInfo.Size = sizeof(p_mesh->Indices[0]) * p_mesh->Indices.size();
		l_indexBufferStagingAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		l_indexBufferStagingAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		VulkanBuffer_alloc(&p_mesh->IndicesStaggingBuffer, &l_indexBufferStagingAllocInfo, p_meshAllocInfo->Device);

		_Memory::VulkanBuffer_pushToGPU(&p_mesh->IndicesStaggingBuffer, p_meshAllocInfo->Device, p_mesh->Indices.data(), static_cast<size_t>(p_mesh->IndicesStaggingBuffer.BufferCreateInfo.size));
	};

	void Mesh_free(Mesh* p_mesh, _Device::Device* p_device)
	{
		VulkanBuffer_free(&p_mesh->VertexBuffer, p_device);
		VulkanBuffer_free(&p_mesh->VertexStaggingBuffer, p_device);
		VulkanBuffer_free(&p_mesh->IndicesBuffer, p_device);
		VulkanBuffer_free(&p_mesh->IndicesStaggingBuffer, p_device);
	};
}


