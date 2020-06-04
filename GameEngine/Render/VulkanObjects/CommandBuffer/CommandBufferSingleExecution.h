#pragma once

#include "CommandBuffers.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
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
			RenderInterface* p_renderInterface, CommandBufferSingleExecutionAllocInfo* p_commandBufferSingleExecutionAllocInfo);
	void CommandBufferSingleExecution_startRecording(CommandBufferSingleExecution* p_commandBufferSingleExecution, RenderInterface* p_renderInterface);
	void CommandBufferSingleExecution_execute(CommandBufferSingleExecution* p_commandBufferSingleExecution, RenderInterface* p_renderInterface);
}