#include "CommandBuffers.h"

#include <stdexcept>

extern "C"
{
#include "Log/LogFormatting.h"
}

#include "VulkanObjects/Hardware/Device/Device.h"
#include "VulkanObjects/Hardware/Device/Device.h"

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
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to allocate command buffers!"));
		}
	};

	void CommandBuffer_free(CommandBuffer* p_commandBuffer, Device* p_device)
	{
		vkFreeCommandBuffers(p_device->LogicalDevice.LogicalDevice, p_commandBuffer->CommandBuffersDependencies.CommandPool->CommandPool, 1, &p_commandBuffer->CommandBuffer);
	};
};