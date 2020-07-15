#include "DeferredCommandBufferOperation.h"
#include <stdlib.h>

namespace _GameEngine::_Render
{
	void smartDeferredCommandBufferCompletionToken_clearReference(SmartDeferredCommandBufferCompletionToken* p_smartToken, void* null)
	{
		p_smartToken->TokenReference = nullptr;
	};

	void SmartDeferredCommandBufferCompletionToken_build(SmartDeferredCommandBufferCompletionToken* p_smartToken, DeferredCommandBufferCompletionToken** p_token)
	{
		auto l_token = *p_token;
		p_smartToken->TokenReference = l_token;
		l_token->OnTokenDestructed = { smartDeferredCommandBufferCompletionToken_clearReference, p_smartToken };
	};

	bool SmartDeferredCommandBufferCompletionToken_isNull(SmartDeferredCommandBufferCompletionToken* p_smartToken)
	{
		return p_smartToken->TokenReference == nullptr;
	};

	void DeferredCommandBufferOperation_alloc(DeferredCommandBufferOperation* p_deferredCommandBufferOperation)
	{
		p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken = (DeferredCommandBufferCompletionToken*)calloc(1, sizeof(DeferredCommandBufferCompletionToken));
	};

	void DeferredCommandBufferOperation_free(DeferredCommandBufferOperation* p_deferredCommandBufferOperation)
	{
		_Core::CallbackT_call(&p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken->OnTokenDestructed, (void*) NULL);
		free(p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken);
		p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken = nullptr;
	};
};