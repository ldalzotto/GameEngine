#include "Mesh.h"

namespace _GameEngine::_Render::_Mesh
{
	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo)
	{
		p_mesh->Vertices = *p_meshAllocInfo->Vertices;

		_Memory::BufferAllocInfo l_vertexBufferAllocInfo{};
		l_vertexBufferAllocInfo.Size = sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
		l_vertexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VertexBuffer_alloc(&p_mesh->VertexBuffer, &l_vertexBufferAllocInfo, p_meshAllocInfo->Device);

		void* data;
		vkMapMemory(p_meshAllocInfo->Device->LogicalDevice.LogicalDevice, p_mesh->VertexBuffer.BufferMemory, 0, p_mesh->VertexBuffer.BufferCreateInfo.size, 0, &data);
		memcpy(data, p_mesh->Vertices.data(), static_cast<size_t>(p_mesh->VertexBuffer.BufferCreateInfo.size));
		vkUnmapMemory(p_meshAllocInfo->Device->LogicalDevice.LogicalDevice, p_mesh->VertexBuffer.BufferMemory);
	};

	void Mesh_free(Mesh* p_mesh, _Device::Device* p_device)
	{
		VertexBuffer_free(&p_mesh->VertexBuffer, p_device);
	};
}


