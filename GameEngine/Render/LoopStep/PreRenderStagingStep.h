#pragma once

#include <vector>

#include "Render/Memory/VulkanBuffer.h"
#include "Render/CommandBuffer/CommandBuffers.h"
#include "Render/CommandBuffer/CommandPool.h"

#include "Render/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	struct PreRenderStaggingOperation
	{
		VulkanBuffer StagingBuffer;
		VulkanBuffer* TargetBuffer;
	};

	struct PreRenderStagingStep
	{
		CommandBuffer DedicatedCommandBuffer;
		VkFence PreRenderStaggingFence;
		std::vector<PreRenderStaggingOperation> StaggingOprtations;
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