#include "CommandBuffers.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render::_CommandBuffer
{
	void CommandBuffers_init(CommandBuffers* p_commandBuffers, CommandBuffersDependencies* p_commandBuffersDependencies)
	{
		p_commandBuffers->CommandBuffersDependencies = *p_commandBuffersDependencies;
		p_commandBuffers->CommandBuffers.resize(p_commandBuffers->CommandBuffersDependencies.FrameBuffer->FrameBuffers.size());

		VkCommandBufferAllocateInfo l_commandBufferAllocateInfo{};
		l_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		l_commandBufferAllocateInfo.commandPool = p_commandBuffers->CommandBuffersDependencies.CommandPool->CommandPool;
		l_commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		l_commandBufferAllocateInfo.commandBufferCount = (uint32_t)p_commandBuffers->CommandBuffers.size();

		if (vkAllocateCommandBuffers(p_commandBuffersDependencies->CommandPool->CommanPoolDependencies.Device->LogicalDevice.LogicalDevice, &l_commandBufferAllocateInfo, p_commandBuffers->CommandBuffers.data())
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to allocate command buffers!"));
		}

		// Starting command buffers
		for (size_t i = 0; i < p_commandBuffers->CommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo l_commandBufferBeginInfo{};
			l_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			l_commandBufferBeginInfo.flags = 0;
			l_commandBufferBeginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(p_commandBuffers->CommandBuffers[i], &l_commandBufferBeginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to begin recording command buffer!"));
			}
		}


	};

	VkFramebuffer get_frameBuffer(GetFrameBufferInfo* p_getFrameBufferInfo)
	{
		return p_getFrameBufferInfo->CommandBuffers->CommandBuffersDependencies.FrameBuffer->FrameBuffers[p_getFrameBufferInfo->CommandBufferIndex];
	};
};