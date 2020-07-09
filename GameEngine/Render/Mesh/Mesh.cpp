#include "Mesh.h"

#include "RenderStep/PreRenderDeferedCommandBufferStep.h"
#include "VulkanObjects/CommandBuffer/DeferredOperations/BufferCopyDeferredOperation.h"

namespace _GameEngine::_Render
{
	Mesh* Mesh_allocProcedural(MeshAllocProceduralInfo* p_meshAllocProceduralInfo)
	{
		Mesh* l_mesh = new Mesh();
		l_mesh->MeshUniqueKey = *p_meshAllocProceduralInfo->MeshUniqueKey;
		l_mesh->Vertices = *p_meshAllocProceduralInfo->Vertices;
		l_mesh->Indices = *p_meshAllocProceduralInfo->Indices;

		{
			BufferAllocInfo l_vertexBufferAllocInfo{};
			l_vertexBufferAllocInfo.Size = sizeof(l_mesh->Vertices[0]) * l_mesh->Vertices.size();
			l_vertexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			l_vertexBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			VulkanBuffer_alloc(&l_mesh->VertexBuffer, &l_vertexBufferAllocInfo, p_meshAllocProceduralInfo->Device);

			VulkanBuffer l_vertexStagingBuffer{};
			BufferAllocInfo l_vertexStaggingBufferAllocInfo{};
			l_vertexStaggingBufferAllocInfo.Size = sizeof(l_mesh->Vertices[0]) * l_mesh->Vertices.size();
			l_vertexStaggingBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			l_vertexStaggingBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			VulkanBuffer_alloc(&l_vertexStagingBuffer, &l_vertexStaggingBufferAllocInfo, p_meshAllocProceduralInfo->Device);
			VulkanBuffer_pushToGPU(&l_vertexStagingBuffer, p_meshAllocProceduralInfo->Device, l_mesh->Vertices.data(), static_cast<size_t>(l_vertexStagingBuffer.BufferAllocInfo.Size));

			BufferCopyOperation* l_bufferCopyOperation = new BufferCopyOperation();
			l_bufferCopyOperation->Device = p_meshAllocProceduralInfo->Device;
			l_bufferCopyOperation->SourceBuffer = l_vertexStagingBuffer;
			l_bufferCopyOperation->TargetBuffer = &l_mesh->VertexBuffer;
			DeferredCommandBufferOperation l_vertexStagingCopyOperation = BufferCopyDeferredOperation_build(&l_bufferCopyOperation);
			SmartDeferredCommandBufferCompletionToken_build(&l_mesh->VerticesStagingBufferCompletionToken, &l_vertexStagingCopyOperation.DeferredCommandBufferCompletionToken);
			p_meshAllocProceduralInfo->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_vertexStagingCopyOperation));

		}

		{
			BufferAllocInfo l_indexBufferAllocInfo{};
			l_indexBufferAllocInfo.Size = sizeof(l_mesh->Indices[0]) * l_mesh->Indices.size();
			l_indexBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			l_indexBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			VulkanBuffer_alloc(&l_mesh->IndicesBuffer, &l_indexBufferAllocInfo, p_meshAllocProceduralInfo->Device);

			VulkanBuffer l_indexStaggingBuffer{};
			BufferAllocInfo l_indexBufferStagingAllocInfo{};
			l_indexBufferStagingAllocInfo.Size = sizeof(l_mesh->Indices[0]) * l_mesh->Indices.size();
			l_indexBufferStagingAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			l_indexBufferStagingAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			VulkanBuffer_alloc(&l_indexStaggingBuffer, &l_indexBufferStagingAllocInfo, p_meshAllocProceduralInfo->Device);
			VulkanBuffer_pushToGPU(&l_indexStaggingBuffer, p_meshAllocProceduralInfo->Device, l_mesh->Indices.data(), static_cast<size_t>(l_indexStaggingBuffer.BufferAllocInfo.Size));

			BufferCopyOperation* l_bufferCopyOperation = new BufferCopyOperation();
			l_bufferCopyOperation->Device = p_meshAllocProceduralInfo->Device;
			l_bufferCopyOperation->SourceBuffer = l_indexStaggingBuffer;
			l_bufferCopyOperation->TargetBuffer = &l_mesh->IndicesBuffer;
			DeferredCommandBufferOperation l_indexStagingCopyOperation = BufferCopyDeferredOperation_build(&l_bufferCopyOperation);
			SmartDeferredCommandBufferCompletionToken_build(&l_mesh->IndicesStagingBufferCompletionToken, &l_indexStagingCopyOperation.DeferredCommandBufferCompletionToken);
			p_meshAllocProceduralInfo->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_indexStagingCopyOperation));
		}

		return l_mesh;
	};

	void Mesh_free(Mesh** p_mesh, Device* p_device)
	{
		Mesh* l_mesh = *p_mesh;

		if (!SmartDeferredCommandBufferCompletionToken_isNull(&l_mesh->IndicesStagingBufferCompletionToken))
		{
			l_mesh->IndicesStagingBufferCompletionToken.TokenReference->IsCancelled = true;
		}
		if (!SmartDeferredCommandBufferCompletionToken_isNull(&l_mesh->VerticesStagingBufferCompletionToken))
		{
			l_mesh->VerticesStagingBufferCompletionToken.TokenReference->IsCancelled = true;
		}
		
		VulkanBuffer_free(&l_mesh->VertexBuffer, p_device);
		VulkanBuffer_free(&l_mesh->IndicesBuffer, p_device);
		delete l_mesh;
		l_mesh = nullptr;
		p_mesh = nullptr;
	};

}


namespace _CoreV3
{
	template <>
	inline size_t Hash<_GameEngine::_Render::MeshUniqueKey>(_GameEngine::_Render::MeshUniqueKey* p_meshUniqueKey)
	{
		return Hash(STR(p_meshUniqueKey->MeshAssetPath.c_str()));
	}
}