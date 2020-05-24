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

	struct PreRenderStagging
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

	void PreRenderStagging_alloc(PreRenderStagging* p_preRenderStagging, CommandPool* p_commandPool);
	void PreRenderStagging_free(PreRenderStagging* p_preRenderStagging, Device* p_device);
	PreRenderStaggingCommandBufferBuildStatusBitFlag PreRenderStagging_buildCommandBuffer(PreRenderStagging* p_preRenderStagging, Device* p_device);
	void PreRenderStagging_WaitForFence(PreRenderStagging* p_preRenderStagging, Device* p_device);
}