#pragma once

#include "vulkan/vulkan.h"
#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

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