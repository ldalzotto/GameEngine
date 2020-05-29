#include "CommandBuffers.h"

#include <stdexcept>
#include "Log/Log.h"

#include "Render/Hardware/Device/Device.h"

namespace _GameEngine::_Render
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
	};
};