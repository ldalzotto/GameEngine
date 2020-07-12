#pragma once

#include "vulkan/vulkan.h"

extern "C"
{
#include "Functional/Callback/Observer.h"
}

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct BeforeEndRecordingMainCommandBuffer_Input
	{
		RenderInterface* RenderInterface;
		VkCommandBuffer CommandBuffer;
		size_t ImageIndex;
	};

	struct RenderHookCallbacks
	{
		Core_Observer BeforeEndRecordingMainCommandBuffer;
	};

	void RenderHookCallbacks_alloc(RenderHookCallbacks* p_hook);
	void RenderHookCallbacks_free(RenderHookCallbacks* p_hook);
}