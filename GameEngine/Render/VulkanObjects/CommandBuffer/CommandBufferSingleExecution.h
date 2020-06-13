#pragma once

#include "CommandBuffers.h"

namespace _GameEngine::_Render
{
	struct CommandPool;
	struct Device;
}

namespace _GameEngine::_Render
{
	struct CommandBufferSingleExecution
	{
		CommandBuffer CommandBuffer;
		VkQueue Queue;
		VkFence ExecutionFence;
	};

	struct CommandBufferSingleExecutionAllocInfo
	{
		VkQueue Queue;
	};

	void CommandBufferSingleExecution_alloc(CommandBufferSingleExecution* p_commandBufferSingleExecution, 
			CommandPool* p_commandPool, Device* p_device, CommandBufferSingleExecutionAllocInfo* p_commandBufferSingleExecutionAllocInfo);
	void CommandBufferSingleExecution_startRecording(CommandBufferSingleExecution* p_commandBufferSingleExecution);
	void CommandBufferSingleExecution_execute(CommandBufferSingleExecution* p_commandBufferSingleExecution, Device* p_device);
}