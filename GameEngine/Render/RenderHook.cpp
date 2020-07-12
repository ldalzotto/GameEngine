#include "RenderHook.h"

namespace _GameEngine::_Render
{
	void RenderHookCallbacks_alloc(RenderHookCallbacks* p_hook)
	{
		Core_ObserverAlloc(&p_hook->BeforeEndRecordingMainCommandBuffer);
	};

	void RenderHookCallbacks_free(RenderHookCallbacks* p_hook)
	{
		Core_ObserverFree(&p_hook->BeforeEndRecordingMainCommandBuffer);
	};
}