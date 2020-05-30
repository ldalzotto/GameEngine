#pragma once

namespace _GameEngine::_Render
{
	struct CommandBuffer;
}

namespace _GameEngine::_Render
{
	struct DeferredCommandBufferCompletionToken
	{
		bool IsCompleted;
		bool IsCancelled;
	};

	struct DeferredCommandBufferOperation;

	typedef void(*DeferredCommandBufferOperation_BuildCommandBuffer)(CommandBuffer*, DeferredCommandBufferOperation* p_commentBufferOperation);
	typedef void(*DeferredCommandBufferOperation_OperationExecuted)(DeferredCommandBufferOperation* p_commentBufferOperation);

	struct DeferredCommandBufferOperation
	{
		DeferredCommandBufferCompletionToken* DeferredCommandBufferCompletionToken;
		void* UserObject;
		DeferredCommandBufferOperation_BuildCommandBuffer BuildCommandBuffer;
		DeferredCommandBufferOperation_OperationExecuted OnOperationExecuted;
	};

	void DeferredCommandBufferOperation_alloc(DeferredCommandBufferOperation* p_deferredCommandBufferOperation);
	
	void DeferredCommandBufferOperation_free(DeferredCommandBufferOperation* p_deferredCommandBufferOperation);
};