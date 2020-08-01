#pragma once

#include <string>
#include <vector>

#include "v2/Vector/Vector.hpp"

#include "VulkanObjects/Memory/VulkanBuffer.h"
#include "VulkanObjects/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
}

namespace _GameEngine::_Render
{
	struct Vertex
	{
		_MathV2::Vector3<float> pos;
		_MathV2::Vector2<float> TexCoord;
	};

	struct MeshUniqueKey
	{
		std::string MeshAssetPath;
	};

	size_t MeshUniqueKey_Hash(MeshUniqueKey* p_key);

	struct Mesh
	{
		MeshUniqueKey MeshUniqueKey;

		std::vector<Vertex> Vertices;
		VulkanBuffer VertexBuffer;
		
		std::vector<uint16_t> Indices;
		VulkanBuffer IndicesBuffer;
		
		SmartDeferredCommandBufferCompletionToken VerticesStagingBufferCompletionToken;
		SmartDeferredCommandBufferCompletionToken IndicesStagingBufferCompletionToken;
	};

	struct MeshAllocProceduralInfo
	{
		MeshUniqueKey* MeshUniqueKey;
		std::vector<Vertex>* Vertices;
		std::vector<uint16_t>* Indices;

		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	Mesh* Mesh_allocProcedural(MeshAllocProceduralInfo* p_meshAllocProceduralInfo);
	void Mesh_free(Mesh** p_mesh, Device* p_device);
}
