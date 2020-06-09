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
		VkImageType ImageType;
		uint32_t MipLevels;
		uint32_t ArrayLayers;
		VkFormat Format;
		VkImageTiling Tiling;
		VkImageUsageFlags Usage;
		VkSharingMode SharingMode;
		VkSampleCountFlagBits Samples;
		uint32_t Width;
		uint32_t Height;
		uint32_t Depth;
	};

	enum class TextureType : uint8_t
	{
		ASSET = 0, PROCEDURAL = 1
	};

	struct Texture
	{
		TextureUniqueKey TextureUniqueKey;
		TextureType TextureType;
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
	
	struct ImageViewCreateInfo
	{
		VkImageViewType            viewType;
		VkImageAspectFlags		   aspectMask;
	};

	struct TextureProceduralInstanceInfo
	{
		TextureUniqueKey* TextureKey;

		TextureInfo* TextureInfo;
		ImageViewCreateInfo ImageViewCreateInfo;
		AllocDeferredCommandBufferOperation AllocDeferredCommandBufferOperation;

		Device* Device;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	Texture* Texture_proceduralInstance(TextureProceduralInstanceInfo* p_textureProceduralInstanceInfo);
	void check_textureValidationToken_undefinedBehavior(_GameEngine::_Render::Texture* l_texture, _GameEngine::_Render::PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep);
	void Texture_free(Texture** p_texture, Device* p_device, PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep);
}