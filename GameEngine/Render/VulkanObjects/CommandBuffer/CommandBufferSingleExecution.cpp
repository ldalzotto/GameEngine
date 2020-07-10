#include "CommandBufferSingleExecution.h"

#include <stdexcept>
extern "C"
{
#include "Log/LogFormatting.h"
}

#include "VulkanObjects/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	void CommandBufferSingleExecution_alloc(CommandBufferSingleExecution* p_commandBufferSingleExecution,
		CommandPool* p_commandPool, Device* p_device, CommandBufferSingleExecutionAllocInfo* p_commandBufferSingleExecutionAllocInfo)
	{
		p_commandBufferSingleExecution->Queue = p_commandBufferSingleExecutionAllocInfo->Queue;


		CommandBuffersDependencies l_commandBufferDependencies{};
		l_commandBufferDependencies.CommandPool = p_commandPool;
		CommandBuffer_init(&p_commandBufferSingleExecution->CommandBuffer, &l_commandBufferDependencies);

		{
			VkFenceCreateInfo l_fenceCreateInfo{};
			l_fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			vkCreateFence(p_device->LogicalDevice.LogicalDevice, &l_fenceCreateInfo, nullptr, &p_commandBufferSingleExecution->ExecutionFence);
		}

	};

	void CommandBufferSingleExecution_startRecording(CommandBufferSingleExecution* p_commandBufferSingleExecution)
	{
		VkCommandBufferBeginInfo l_commandBufferBeginInfo{};
		l_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		l_commandBufferBeginInfo.flags = 0;
		l_commandBufferBeginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(p_commandBufferSingleExecution->CommandBuffer.CommandBuffer, &l_commandBufferBeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to begin recording command buffer!"));
		}

	};

	void CommandBufferSingleExecution_execute(CommandBufferSingleExecution* p_commandBufferSingleExecution, Device* p_device)
	{
		if (vkEndCommandBuffer(p_commandBufferSingleExecution->CommandBuffer.CommandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to record command buffer!"));
		}

		VkSubmitInfo l_staginSubmit{};
		l_staginSubmit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		l_staginSubmit.commandBufferCount = 1;
		l_staginSubmit.pCommandBuffers = &p_commandBufferSingleExecution->CommandBuffer.CommandBuffer;
		vkQueueSubmit(p_commandBufferSingleExecution->Queue, 1, &l_staginSubmit, p_commandBufferSingleExecution->ExecutionFence);

		vkWaitForFences(p_device->LogicalDevice.LogicalDevice, 1, &p_commandBufferSingleExecution->ExecutionFence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(p_device->LogicalDevice.LogicalDevice, p_commandBufferSingleExecution->ExecutionFence, nullptr);
		p_commandBufferSingleExecution->ExecutionFence = VK_NULL_HANDLE;
		CommandBuffer_free(&p_commandBufferSingleExecution->CommandBuffer, p_device);
	};
}