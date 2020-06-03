#pragma once

#include "DeferredCommandBufferOperation.h"
#include "Render/VulkanObjects/Memory/VulkanBuffer.h"


namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct BufferCopyOperation
	{
		VulkanBuffer SourceBuffer;
		VulkanBuffer* TargetBuffer;
		Device* Device;
	};

	DeferredCommandBufferOperation BufferCopyDeferredOperation_build(BufferCopyOperation** p_bufferCopyOperation);
};