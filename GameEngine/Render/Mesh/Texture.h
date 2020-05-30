#pragma once

#include <string>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
	struct DeferredCommandBufferCompletionToken;
}

namespace _GameEngine::_Render
{
	struct TextureInfo
	{
		uint32_t MipLevels;
		uint32_t ArrayLayers;
		VkFormat Format;
		uint32_t Width;
		uint32_t Height;
		uint32_t Depth;
	};

	struct Texture
	{
		TextureInfo TextureInfo;
		VkImage Texture;
		VkDeviceMemory TextureMemory;

		DeferredCommandBufferCompletionToken* TextureInitializationBufferCompletionToken;
	};

	struct TextureLoadInfo
	{
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	void Texture_load(Texture* p_texture,const std::string& l_texturePath, TextureLoadInfo* l_textureLoadInfo);
	void Texture_free(Texture* p_texture, Device* p_device);
}