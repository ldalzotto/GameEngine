#include "CommandBuffers.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render::_CommandBuffer
{
	void CommandBuffer_init(CommandBuffer* p_commandBuffer, CommandBuffersDependencies* p_commandBuffersDependencies)
	{
		p_commandBuffer->CommandBuffersDependencies = *p_commandBuffersDependencies;

		VkCommandBufferAllocateInfo l_commandBufferAllocateInfo{};
		l_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		l_commandBufferAllocateInfo.commandPool = p_commandBuffer->CommandBuffersDependencies.CommandPool->CommandPool;
		l_commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		l_commandBufferAllocateInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(p_commandBuffersDependencies->CommandPool->CommanPoolDependencies.Device->LogicalDevice.LogicalDevice, &l_commandBufferAllocateInfo, &p_commandBuffer->CommandBuffer)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to allocate command buffers!"));
		}

		// Starting command buffer
		VkCommandBufferBeginInfo l_commandBufferBeginInfo{};
		l_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		l_commandBufferBeginInfo.flags = 0;
		l_commandBufferBeginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(p_commandBuffer->CommandBuffer, &l_commandBufferBeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to begin recording command buffer!"));
		}
	};
};