#pragma once

#include "Utils/Observer/Callback.h"

namespace _GameEngine::_Render
{
	struct CommandBuffer;
}

namespace _GameEngine::_Render
{


	/**
		Completion tokens are automatically destroyed when the operation is either completed or aborted.
		See @ref DeferredCommandBufferOperation_free method.
		When destroyed, the @ref OnTokenDestructed callback is called
	*/
	struct DeferredCommandBufferCompletionToken
	{
		bool IsCompleted;
		bool IsCancelled;
		_Utils::Callback OnTokenDestructed;
	};

	/**
		The @ref SmartDeferredCommandBufferCompletionToken has been created to automatically manage the nullification of a DeferredCommandBufferCompletionToken reference when destructed. 
	*/
	struct SmartDeferredCommandBufferCompletionToken
	{
		DeferredCommandBufferCompletionToken* TokenReference;
	};

	void SmartDeferredCommandBufferCompletionToken_build(SmartDeferredCommandBufferCompletionToken* p_smartToken, DeferredCommandBufferCompletionToken** p_token);
	bool SmartDeferredCommandBufferCompletionToken_isNull(SmartDeferredCommandBufferCompletionToken* p_smartToken);

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