#pragma once


#include "Render/CommandBuffer/CommandPool.h"

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
};