#include "RenderHook.h"

namespace _GameEngine::_Render
{
	void RenderHookCallbacks_alloc(RenderHookCallbacks* p_hook)
	{
		_Core::ObserverT_alloc(&p_hook->BeforeEndRecordingMainCommandBuffer);
	};

	void RenderHookCallbacks_free(RenderHookCallbacks* p_hook)
	{
		_Core::ObserverT_free(&p_hook->BeforeEndRecordingMainCommandBuffer);
	};
}