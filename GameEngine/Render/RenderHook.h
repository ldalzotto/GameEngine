#pragma once

#include "vulkan/vulkan.h"

#include "Utils/Observer/Observer.h"

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
		_Utils::Observer BeforeEndRecordingMainCommandBuffer;
	};
}