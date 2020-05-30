#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct TextureSamplers
	{
		VkSampler DefaultSampler;
	};

	void TextureSamplers_alloc(TextureSamplers* p_textureSamplers, Device* p_device);
	void TextureSamplers_free(TextureSamplers* p_textureSamplers, Device* p_device);
}