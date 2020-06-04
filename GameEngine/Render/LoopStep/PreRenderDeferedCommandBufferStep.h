#pragma once

#include <vector>

#include "VulkanObjects/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"
#include "VulkanObjects/CommandBuffer/CommandBuffers.h"
#include "VulkanObjects/CommandBuffer/CommandPool.h"

namespace _GameEngine::_Render
{
	struct PreRenderDeferedCommandBufferStep
	{
		CommandBuffer DedicatedCommandBuffer;
		VkFence PreRenderStaggingFence;
		std::vector<DeferredCommandBufferOperation> DefferedOperations;
	};

	enum PreRenderDeferredCommandBufferStepStatusBitFlag
	{
		CREATED = 0x01,
		NOTHING = 0x02
	};

	void PreRenderDeferedCommandBufferStep_alloc(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, CommandPool* p_commandPool);
	void PreRenderDeferedCommandBufferStep_free(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, Device* p_device);
	PreRenderDeferredCommandBufferStepStatusBitFlag PreRenderDeferedCommandBufferStep_buildCommandBuffer(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, Device* p_device);
	void PreRenderDeferedCommandBufferStep_WaitForFence(PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep, Device* p_device);
}