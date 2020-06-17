#pragma once

#include <string>
#include <vector>

#include "Math/Vector/Vector.h"

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
		_Math::Vector3f pos;
		_Math::Vector2f TexCoord;
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