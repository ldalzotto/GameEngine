#pragma once

#include <string>

#include "vulkan/vulkan.h"
#include "ImageViews.h"

#include "VulkanObjects/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct PreRenderDeferedCommandBufferStep;
	struct DeferredCommandBufferCompletionToken;
	struct DeferredCommandBufferOperation;
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

		SmartDeferredCommandBufferCompletionToken TextureInitializationBufferCompletionToken;
	};


	struct TextureLoadInfo
	{
		TextureUniqueKey* TextureKey;
		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	Texture* Texture_loadFromFile(TextureLoadInfo* l_textureLoadInfo);

	typedef void(*AllocDeferredCommandBufferOperation)(DeferredCommandBufferOperation* p_deferredCommandBufferOperation, Texture* p_texture);

	struct TextureProceduralCreateInfo
	{
		VkImageType              imageType;
		uint32_t                 mipLevels;
		uint32_t                 arrayLayers;
		VkFormat                 format;
		VkImageTiling            tiling;
		VkImageLayout            initialLayout;
		VkImageUsageFlags        usage;
		VkSharingMode            sharingMode;
		VkSampleCountFlagBits    samples;
		VkImageCreateFlags       flags;
	};

	struct TextureProceduralInstanceInfo
	{
		TextureUniqueKey* TextureKey;
		uint32_t Width;
		uint32_t Height;

		TextureProceduralCreateInfo TextureProceduralCreateInfo;
		ImageViewCreationInfoProvider ImageViewCreationInfoProvider;
		AllocDeferredCommandBufferOperation AllocDeferredCommandBufferOperation;

		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	Texture* Texture_proceduralInstance(TextureProceduralInstanceInfo* p_textureProceduralInstanceInfo);
	void check_textureValidationToken_undefinedBehavior(_GameEngine::_Render::Texture* l_texture, _GameEngine::_Render::PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep);
	void Texture_free(Texture** p_texture, Device* p_device, PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep);
}