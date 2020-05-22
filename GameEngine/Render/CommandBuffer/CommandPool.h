#pragma once

#include "vulkan/vulkan.h"
#include "Render/Device/Device.h"

namespace _GameEngine::_Render::_CommandBuffer
{

	struct CommandPoolDependencies
	{
		_Device::Device* Device;
	};

	struct CommandPool
	{
		CommandPoolDependencies CommanPoolDependencies;
		VkCommandPool CommandPool;
	};

	void CommandPool_Init(CommandPool* p_commandPool, CommandPoolDependencies* p_commandPoolDependencies);
	void CommandPool_free(CommandPool* p_commandPool);
};