#include "Mesh.h"

namespace _GameEngine::_Render::_Mesh
{
	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo)
	{
		p_mesh->Vertices = *p_meshAllocInfo->Vertices;
		p_mesh->Indices = *p_meshAllocInfo->Indices;

		_Memory::BufferAllocInfo l_vertexBufferAllocInfo{};
		l_vertexBufferAllocInfo.Size = sizeof(p_mesh->Vertices[0]) * p_mesh->Vertices.size();
		l_vertexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VertexBuffer_alloc(&p_mesh->VertexBuffer, &l_vertexBufferAllocInfo, p_meshAllocInfo->Device);
		_Memory::VertexBuffer_pushToGPU(&p_mesh->VertexBuffer, p_meshAllocInfo->Device, p_mesh->Vertices.data(), static_cast<size_t>(p_mesh->VertexBuffer.BufferCreateInfo.size));

		_Memory::BufferAllocInfo l_indexBufferAllocInfo{};
		l_indexBufferAllocInfo.Size = sizeof(p_mesh->Indices[0]) * p_mesh->Indices.size();
		l_indexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		VertexBuffer_alloc(&p_mesh->IndicesBuffer, &l_indexBufferAllocInfo, p_meshAllocInfo->Device);
		_Memory::VertexBuffer_pushToGPU(&p_mesh->IndicesBuffer, p_meshAllocInfo->Device, p_mesh->Indices.data(), static_cast<size_t>(p_mesh->IndicesBuffer.BufferCreateInfo.size));
	};

	void Mesh_free(Mesh* p_mesh, _Device::Device* p_device)
	{
		VertexBuffer_free(&p_mesh->VertexBuffer, p_device);
		VertexBuffer_free(&p_mesh->IndicesBuffer, p_device);
	};
}


