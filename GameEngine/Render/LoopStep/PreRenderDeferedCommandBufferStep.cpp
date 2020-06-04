#include "PreRenderDeferedCommandBufferStep.h"

#include <stdexcept>
#include "Log/Log.h"

#include "VulkanObjects/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{

	void PreRenderDeferedCommandBufferStep_alloc(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, CommandPool* p_commandPool)
	{
		CommandBuffersDependencies l_commandBufferDependencies{};
		l_commandBufferDependencies.CommandPool = p_commandPool;
		CommandBuffer_init(&p_preRenderDeferedCommandBufferStep->DedicatedCommandBuffer, &l_commandBufferDependencies);
	};
	
	void PreRenderDeferedCommandBufferStep_free(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, Device* p_device)
	{
		if (p_preRenderDeferedCommandBufferStep->PreRenderStaggingFence != VK_NULL_HANDLE)
		{
			vkDestroyFence(p_device->LogicalDevice.LogicalDevice, p_preRenderDeferedCommandBufferStep->PreRenderStaggingFence, nullptr);
		}
	};

	PreRenderDeferredCommandBufferStepStatusBitFlag PreRenderDeferedCommandBufferStep_buildCommandBuffer(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, Device* p_device)
	{
		if (p_preRenderDeferedCommandBufferStep->DefferedOperations.size() > 0)
		{
			VkCommandBufferBeginInfo l_commandBufferBeginInfo{};
			l_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			l_commandBufferBeginInfo.flags = 0;
			l_commandBufferBeginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(p_preRenderDeferedCommandBufferStep->DedicatedCommandBuffer.CommandBuffer, &l_commandBufferBeginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to begin recording command buffer!"));
			}

			VkFenceCreateInfo l_staginFenceCreate{};
			l_staginFenceCreate.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			vkCreateFence(p_device->LogicalDevice.LogicalDevice, &l_staginFenceCreate, nullptr, &p_preRenderDeferedCommandBufferStep->PreRenderStaggingFence);

			for (DeferredCommandBufferOperation& l_deferredCommandBufferOperation : p_preRenderDeferedCommandBufferStep->DefferedOperations)
			{
				if (l_deferredCommandBufferOperation.DeferredCommandBufferCompletionToken)
				{
					if (!l_deferredCommandBufferOperation.DeferredCommandBufferCompletionToken->IsCancelled)
					{
						l_deferredCommandBufferOperation.BuildCommandBuffer(&p_preRenderDeferedCommandBufferStep->DedicatedCommandBuffer, &l_deferredCommandBufferOperation);
						l_deferredCommandBufferOperation.DeferredCommandBufferCompletionToken->IsCompleted = true;
					}
				}
				else
				{
					l_deferredCommandBufferOperation.BuildCommandBuffer(&p_preRenderDeferedCommandBufferStep->DedicatedCommandBuffer, &l_deferredCommandBufferOperation);
				}
				
			}

			if (vkEndCommandBuffer(p_preRenderDeferedCommandBufferStep->DedicatedCommandBuffer.CommandBuffer) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to record command buffer!"));
			}

			return PreRenderDeferredCommandBufferStepStatusBitFlag::CREATED;
		}
		else
		{
			return PreRenderDeferredCommandBufferStepStatusBitFlag::NOTHING;
		}
	};

	void PreRenderDeferedCommandBufferStep_WaitForFence(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, Device* p_device)
	{
		vkWaitForFences(p_device->LogicalDevice.LogicalDevice, 1, &p_preRenderDeferedCommandBufferStep->PreRenderStaggingFence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(p_device->LogicalDevice.LogicalDevice, p_preRenderDeferedCommandBufferStep->PreRenderStaggingFence, nullptr);
		p_preRenderDeferedCommandBufferStep->PreRenderStaggingFence = VK_NULL_HANDLE;

		for (DeferredCommandBufferOperation& l_deferredCommandBufferOperation : p_preRenderDeferedCommandBufferStep->DefferedOperations)
		{
			if (l_deferredCommandBufferOperation.OnOperationExecuted)
			{
				l_deferredCommandBufferOperation.OnOperationExecuted(&l_deferredCommandBufferOperation);
			}

			DeferredCommandBufferOperation_free(&l_deferredCommandBufferOperation);
		}

		p_preRenderDeferedCommandBufferStep->DefferedOperations.clear();
	};

}