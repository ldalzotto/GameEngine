#include "PreRenderStagingStep.h"

#include <stdexcept>
#include "Log/Log.h"

#include "Render/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{

	void preRenderStaggingOperationValidation(PreRenderStaggingOperation* p_preRenderStagingOperation);

	void PreRenderStagingStep_alloc(PreRenderStagingStep* p_preRenderStagging, CommandPool* p_commandPool)
	{
		CommandBuffersDependencies l_commandBufferDependencies{};
		l_commandBufferDependencies.CommandPool = p_commandPool;
		CommandBuffer_init(&p_preRenderStagging->DedicatedCommandBuffer, &l_commandBufferDependencies);
	};

	void PreRenderStagingStep_free(PreRenderStagingStep* p_preRenderStagging, Device* p_device)
	{
		if (p_preRenderStagging->PreRenderStaggingFence != VK_NULL_HANDLE)
		{
			vkDestroyFence(p_device->LogicalDevice.LogicalDevice, p_preRenderStagging->PreRenderStaggingFence, nullptr);
		}
	};

	PreRenderStaggingCommandBufferBuildStatusBitFlag PreRenderStagingStep_buildCommandBuffer(PreRenderStagingStep* p_preRenderStagging, Device* p_device)
	{
		if (p_preRenderStagging->StaggingOperations.size() > 0)
		{
			VkCommandBufferBeginInfo l_commandBufferBeginInfo{};
			l_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			l_commandBufferBeginInfo.flags = 0;
			l_commandBufferBeginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(p_preRenderStagging->DedicatedCommandBuffer.CommandBuffer, &l_commandBufferBeginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to begin recording command buffer!"));
			}

			VkFenceCreateInfo l_staginFenceCreate{};
			l_staginFenceCreate.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			vkCreateFence(p_device->LogicalDevice.LogicalDevice, &l_staginFenceCreate, nullptr, &p_preRenderStagging->PreRenderStaggingFence);

			for (PreRenderStaggingOperation& l_staggingOperation : p_preRenderStagging->StaggingOperations)
			{
				if (!l_staggingOperation.StaggingOperationCancelled)
				{
					preRenderStaggingOperationValidation(&l_staggingOperation);
					VkBufferCopy l_bufferCopyInfo{};
					l_bufferCopyInfo.dstOffset = 0;
					l_bufferCopyInfo.srcOffset = 0;
					l_bufferCopyInfo.size = l_staggingOperation.TargetBuffer->BufferAllocInfo.Size;
					vkCmdCopyBuffer(p_preRenderStagging->DedicatedCommandBuffer.CommandBuffer, l_staggingOperation.StagingBuffer.Buffer, l_staggingOperation.TargetBuffer->Buffer, 1, &l_bufferCopyInfo);
				}
			}

			if (vkEndCommandBuffer(p_preRenderStagging->DedicatedCommandBuffer.CommandBuffer) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to record command buffer!"));
			}

			return PreRenderStaggingCommandBufferBuildStatusBitFlag::CREATED;
		}
		else
		{
			return PreRenderStaggingCommandBufferBuildStatusBitFlag::NOTHING;
		}
	};

	void PreRenderStagingStep_WaitForFence(PreRenderStagingStep* p_preRenderStagging, Device* p_device)
	{
		vkWaitForFences(p_device->LogicalDevice.LogicalDevice, 1, &p_preRenderStagging->PreRenderStaggingFence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(p_device->LogicalDevice.LogicalDevice, p_preRenderStagging->PreRenderStaggingFence, nullptr);
		p_preRenderStagging->PreRenderStaggingFence = VK_NULL_HANDLE;

		for (PreRenderStaggingOperation& l_staggingOperation : p_preRenderStagging->StaggingOperations)
		{
			VulkanBuffer_free(&l_staggingOperation.StagingBuffer, p_device);
			if (l_staggingOperation.PrerenderStaggingOperationCompleted.OnStaggingDone)
			{
				l_staggingOperation.PrerenderStaggingOperationCompleted.OnStaggingDone(l_staggingOperation.PrerenderStaggingOperationCompleted.Closure);
			}
		}

		p_preRenderStagging->StaggingOperations.clear();
	};

	void preRenderStaggingOperationValidation(PreRenderStaggingOperation* p_preRenderStagingOperation)
	{

#ifndef NDEBUG
		if (p_preRenderStagingOperation)
		{
			VkMemoryPropertyFlags l_staggingBufferMemoryPropertyFlags = p_preRenderStagingOperation->StagingBuffer.BufferAllocInfo.MemoryPropertyFlags;
			if (!(l_staggingBufferMemoryPropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("The stagging buffer used on a PreRenderStagging step must have a memory allocated with the usage flag VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT."));
			}
		}
#endif
	};
}