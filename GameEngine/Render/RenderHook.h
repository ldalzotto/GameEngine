#pragma once

#include "vulkan/vulkan.h"

#include "Functional/Callback/ObserverT.hpp"

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
		_Core::ObserverT<BeforeEndRecordingMainCommandBuffer_Input> BeforeEndRecordingMainCommandBuffer;
	};

	void RenderHookCallbacks_alloc(RenderHookCallbacks* p_hook);
	void RenderHookCallbacks_free(RenderHookCallbacks* p_hook);
}