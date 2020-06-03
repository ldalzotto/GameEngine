#include "DeferredCommandBufferOperation.h"

namespace _GameEngine::_Render
{
	void smartDeferredCommandBufferCompletionToken_clearReference(void* p_smartToken, void* null)
	{
		auto l_smartToken = (SmartDeferredCommandBufferCompletionToken*)p_smartToken;
		l_smartToken->TokenReference = nullptr;
	};

	void SmartDeferredCommandBufferCompletionToken_build(SmartDeferredCommandBufferCompletionToken* p_smartToken, DeferredCommandBufferCompletionToken** p_token)
	{
		auto l_token = *p_token;
		p_smartToken->TokenReference = l_token;
		l_token->OnTokenDestructed.Closure = p_smartToken;
		l_token->OnTokenDestructed.Function = smartDeferredCommandBufferCompletionToken_clearReference;
	};

	bool SmartDeferredCommandBufferCompletionToken_isNull(SmartDeferredCommandBufferCompletionToken* p_smartToken)
	{
		return p_smartToken->TokenReference == nullptr;
	};

	void DeferredCommandBufferOperation_alloc(DeferredCommandBufferOperation* p_deferredCommandBufferOperation)
	{
		p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken = new DeferredCommandBufferCompletionToken();
	};

	void DeferredCommandBufferOperation_free(DeferredCommandBufferOperation* p_deferredCommandBufferOperation)
	{
		_Utils::Callback_invoke(&p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken->OnTokenDestructed, nullptr);
		delete p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken;
		p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken = nullptr;
	};
};