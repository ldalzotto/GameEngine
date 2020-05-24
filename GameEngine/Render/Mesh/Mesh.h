#pragma once


#include <vector>
#include "glm/glm.hpp"
#include "Render/Memory/VulkanBuffer.h"

#include "Render/Hardware/Device/Device.h"


namespace _GameEngine::_Render
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
	};

	struct Mesh
	{
		std::vector<Vertex> Vertices;
		VulkanBuffer VertexBuffer;
		VulkanBuffer VertexStaggingBuffer;
		
		std::vector<uint16_t> Indices;
		VulkanBuffer IndicesBuffer;
		VulkanBuffer IndicesStaggingBuffer;
	};

	struct MeshAllocInfo
	{
		std::vector<Vertex>* Vertices;
		std::vector<uint16_t>* Indices;
		Device* Device;
	};

	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo);
	void Mesh_free(Mesh* p_mesh, Device* p_device);
}