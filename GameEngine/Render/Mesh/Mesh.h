#pragma once


#include <vector>
#include "glm/glm.hpp"

#include "Render/Memory/VulkanBuffer.h"
#include "Render/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
}

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
		
		std::vector<uint16_t> Indices;
		VulkanBuffer IndicesBuffer;
		
		DeferredCommandBufferCompletionToken* VerticesStagingBufferCompletionToken;
		DeferredCommandBufferCompletionToken* IndicesStagingBufferCompletionToken;
	};

	struct MeshAllocInfo
	{
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo);
	void Mesh_free(Mesh* p_mesh, Device* p_device);
}