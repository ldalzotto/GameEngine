#pragma once

#include <vector>

#include "Render/Memory/VulkanBuffer.h"
#include "Render/CommandBuffer/CommandBuffers.h"
#include "Render/CommandBuffer/CommandPool.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{ 

	struct PrerenderStaggingOperationCompleted
	{
		void(*OnStaggingDone)(void*);
		void* Closure;
	};

	struct PreRenderStaggingOperation
	{
		VulkanBuffer StagingBuffer;
		VulkanBuffer* TargetBuffer;
		bool StaggingOperationCancelled;
		PrerenderStaggingOperationCompleted PrerenderStaggingOperationCompleted;
	};

	struct PreRenderStagingStep
	{
		CommandBuffer DedicatedCommandBuffer;
		VkFence PreRenderStaggingFence;
		std::vector<PreRenderStaggingOperation> StaggingOperations;
	};

	enum PreRenderStaggingCommandBufferBuildStatusBitFlag
	{
		CREATED = 0x01,
		NOTHING = 0x02
	};

	void PreRenderStagingStep_alloc(PreRenderStagingStep* p_preRenderStagging, CommandPool* p_commandPool);
	void PreRenderStagingStep_free(PreRenderStagingStep* p_preRenderStagging, Device* p_device);
	PreRenderStaggingCommandBufferBuildStatusBitFlag PreRenderStagingStep_buildCommandBuffer(PreRenderStagingStep* p_preRenderStagging, Device* p_device);
	void PreRenderStagingStep_WaitForFence(PreRenderStagingStep* p_preRenderStagging, Device* p_device);
}