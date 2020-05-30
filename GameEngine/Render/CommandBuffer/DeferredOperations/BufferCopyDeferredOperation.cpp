#include "BufferCopyDeferredOperation.h"

#include "Render/CommandBuffer/CommandBuffers.h"

namespace _GameEngine::_Render
{
	void BufferCopyDeferredOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation);
	void BufferCopyDeferredOperation_onCompleted(DeferredCommandBufferOperation* p_commentBufferOperation);

	DeferredCommandBufferOperation BufferCopyDeferredOperation_build(BufferCopyOperation** p_bufferCopyOperation)
	{
		DeferredCommandBufferOperation l_defferedCommandBufferOperation{};
		DeferredCommandBufferOperation_alloc(&l_defferedCommandBufferOperation);
		l_defferedCommandBufferOperation.UserObject = *p_bufferCopyOperation;
		l_defferedCommandBufferOperation.BuildCommandBuffer = BufferCopyDeferredOperation_buildCommandBuffer;
		l_defferedCommandBufferOperation.OnOperationExecuted = BufferCopyDeferredOperation_onCompleted;
		return l_defferedCommandBufferOperation;
	}

	void BufferCopyDeferredOperation_buildCommandBuffer(CommandBuffer* p_commandBuffer, DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		BufferCopyOperation* l_bufferCopyOperation = (BufferCopyOperation*)p_commentBufferOperation->UserObject;

		VkBufferCopy l_bufferCopyInfo{};
		l_bufferCopyInfo.dstOffset = 0;
		l_bufferCopyInfo.srcOffset = 0;
		l_bufferCopyInfo.size = l_bufferCopyOperation->TargetBuffer->BufferAllocInfo.Size;
		vkCmdCopyBuffer(p_commandBuffer->CommandBuffer, l_bufferCopyOperation->SourceBuffer.Buffer, l_bufferCopyOperation->TargetBuffer->Buffer, 1, &l_bufferCopyInfo);
	};

	void BufferCopyDeferredOperation_onCompleted(DeferredCommandBufferOperation* p_commentBufferOperation)
	{
		BufferCopyOperation* l_bufferCopyOperation = (BufferCopyOperation*)p_commentBufferOperation->UserObject;
		VulkanBuffer_free(&l_bufferCopyOperation->SourceBuffer, l_bufferCopyOperation->Device);
		delete l_bufferCopyOperation;
	};
};