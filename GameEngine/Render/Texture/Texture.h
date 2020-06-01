#pragma once

#include <string>

#include "vulkan/vulkan.h"
#include "ImageViews.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
	struct DeferredCommandBufferCompletionToken;
}

namespace _GameEngine::_Render
{

	struct TextureUniqueKey
	{
		std::string TexturePath;
	};

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
		TextureUniqueKey TextureUniqueKey;
		TextureInfo TextureInfo;
		VkImage Texture;
		VkDeviceMemory TextureMemory;
		ImageView ImageView;

		DeferredCommandBufferCompletionToken* TextureInitializationBufferCompletionToken;
	};

	struct TextureLoadInfo
	{
		TextureUniqueKey* TextureKey;
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};
	
	Texture* Texture_alloc(TextureLoadInfo* l_textureLoadInfo);
	void Texture_free(Texture** p_texture, Device* p_device);
}