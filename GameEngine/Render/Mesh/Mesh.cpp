#include "Mesh.h"

namespace _GameEngine::_Render::_Mesh
{
	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo)
	{
		p_mesh->Vertices = *p_meshAllocInfo->Vertices;
		VertexBuffer_alloc(&p_mesh->VertexBuffer, sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size(), p_meshAllocInfo->Device);

		void* data;
		vkMapMemory(p_meshAllocInfo->Device->LogicalDevice.LogicalDevice, p_mesh->VertexBuffer.VertexBufferMemory, 0, p_mesh->VertexBuffer.VkBufferCreateInfo.size, 0, &data);
		memcpy(data, p_mesh->Vertices.data(), static_cast<size_t>(p_mesh->VertexBuffer.VkBufferCreateInfo.size));
		vkUnmapMemory(p_meshAllocInfo->Device->LogicalDevice.LogicalDevice, p_mesh->VertexBuffer.VertexBufferMemory);
	};

	void Mesh_free(Mesh* p_mesh, _Device::Device* p_device)
	{
		VertexBuffer_free(&p_mesh->VertexBuffer, p_device);
	};
}


