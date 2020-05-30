#include "DeferredCommandBufferOperation.h"

namespace _GameEngine::_Render
{
	void DeferredCommandBufferOperation_alloc(DeferredCommandBufferOperation* p_deferredCommandBufferOperation)
	{
		p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken = new DeferredCommandBufferCompletionToken();
	};

	void DeferredCommandBufferOperation_free(DeferredCommandBufferOperation* p_deferredCommandBufferOperation)
	{
		delete p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken;
		p_deferredCommandBufferOperation->DeferredCommandBufferCompletionToken = nullptr;
	};
};