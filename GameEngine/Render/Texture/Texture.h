#pragma once

#include <string>

#include "vulkan/vulkan.h"
#include "ImageViews.h"

#include "VulkanObjects/CommandBuffer/DeferredOperations/DeferredCommandBufferOperation.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
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

	enum class TextureAllocationType : uint8_t
	{
		FILE, PROCEDURAL
	};

	enum class TextureType : uint8_t
	{
		COLOR, DEPTH
	};

	enum class TextureUsage : uint8_t
	{
		SHADER_INPUT, PIPELINE_ATTACHMENT
	};

	struct TextureCreateInfo
	{
		TextureType TextureType; /** Is the texture is a color texture or depth ? */
		TextureUsage TextureUsage; /** Will the texture be used in shader ? Or in the graphics pipeline ? */
		uint32_t Width;
		uint32_t Height;
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

	struct TextureAllocInfo
	{
		TextureAllocationType TextureAllocationType;
		TextureUniqueKey* TextureKey;
		TextureCreateInfo TextureCreateInfo;
		RenderInterface* RenderInterface;
	};

	Texture* Texture_alloc(TextureAllocInfo* p_textureAllocInfo);
	void Texture_alloc(Texture** p_texture, TextureAllocInfo* p_textureAllocInfo);

	void Texture_free(Texture** p_texture, RenderInterface* p_renderInterface);
}