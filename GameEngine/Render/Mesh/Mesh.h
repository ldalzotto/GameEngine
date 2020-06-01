#pragma once

#include <string>
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
		glm::vec2 TexCoord;
	};

	struct MeshUniqueKey
	{
		std::string MeshAssetPath;
	};

	struct Mesh
	{
		MeshUniqueKey MeshUniqueKey;

		std::vector<Vertex> Vertices;
		VulkanBuffer VertexBuffer;
		
		std::vector<uint16_t> Indices;
		VulkanBuffer IndicesBuffer;
		
		DeferredCommandBufferCompletionToken* VerticesStagingBufferCompletionToken;
		DeferredCommandBufferCompletionToken* IndicesStagingBufferCompletionToken;
	};

	struct MeshAllocInfo
	{
		MeshUniqueKey* MeshUniqueKey;
		std::vector<Vertex>* Vertices;
		std::vector<uint16_t>* Indices;

		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	Mesh* Mesh_alloc(MeshAllocInfo* p_meshAllocInfo);
	void Mesh_free(Mesh** p_mesh, Device* p_device);
}