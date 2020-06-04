#pragma once


#include "VulkanObjects/CommandBuffer/CommandPool.h"


namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{

	struct CommandBuffersDependencies
	{
		CommandPool* CommandPool;
	};

	struct CommandBuffer
	{
		CommandBuffersDependencies CommandBuffersDependencies;
		VkCommandBuffer CommandBuffer;
	};
	 
	void CommandBuffer_init(CommandBuffer* p_commandBuffers, CommandBuffersDependencies* p_commandBuffersDependencies);
	void CommandBuffer_free(CommandBuffer* p_commandBuffer, Device* p_device);
};