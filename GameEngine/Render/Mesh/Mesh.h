#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Render/Memory/VulkanBuffer.h"

#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

namespace _GameEngine::_Render::_Mesh
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
	};

	struct Mesh
	{
		std::vector<Vertex> Vertices;

		//TODO - As an optimisation, instead of allocating small VulkanBuffer, we can allocate a huge obect and the VulkanBuffer will
		//		 refer to an already created memory address.
		_Memory::VulkanBuffer VertexBuffer;

		
		std::vector<uint16_t> Indices;
		_Memory::VulkanBuffer IndicesBuffer;
	};

	struct MeshAllocInfo
	{
		std::vector<Vertex>* Vertices;
		std::vector<uint16_t>* Indices;
		_Device::Device* Device;
	};

	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo);
	void Mesh_free(Mesh* p_mesh, _Device::Device* p_device);
}