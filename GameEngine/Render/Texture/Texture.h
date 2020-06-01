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

		// Disposed by it's owner @ref DeferredCommandBufferOperation
		DeferredCommandBufferCompletionToken* TextureInitializationBufferCompletionToken;
	};

	typedef VkImageCreateInfo(*VkImageCreateInfoProvider)(uint32_t p_width, uint32_t p_height);

	struct TextureLoadInfo
	{
		TextureUniqueKey* TextureKey;
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};
	
	Texture* Texture_loadFromFile(TextureLoadInfo* l_textureLoadInfo);

	struct TextureProceduralInstanceInfo
	{
		TextureUniqueKey* TextureKey;
		Device* Device;
		uint32_t Width;
		uint32_t Height;
		VkImageCreateInfoProvider ImageCreateInfoProvider;
		ImageViewCreationInfoProvider ImageViewCreationInfoProvider;
	};

	Texture* Texture_porceduralInstance(TextureProceduralInstanceInfo* p_textureProceduralInstanceInfo);

	void Texture_free(Texture** p_texture, Device* p_device);
}