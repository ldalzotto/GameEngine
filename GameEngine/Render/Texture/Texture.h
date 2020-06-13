#pragma once

#include <string>
#include <unordered_map>

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
		VkImageAspectFlags AspectMask;
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

	struct TextureFeatures
	{
		bool FitSwapChainSizeEnabled;
	};

	struct Texture
	{
		TextureUniqueKey TextureUniqueKey;
		TextureInfo TextureInfo;
		TextureFeatures Features;

		VkImage Texture;
		VkDeviceMemory TextureMemory;
		ImageView ImageView;

		SmartDeferredCommandBufferCompletionToken TextureInitializationBufferCompletionToken;
	};

	struct TextureAllocInfo
	{
		TextureAllocationType TextureAllocationType;
		TextureUniqueKey TextureKey;
		TextureCreateInfo TextureCreateInfo;
		bool FitSwapChainSize;

		RenderInterface* RenderInterface;
	};

	Texture* Texture_alloc(TextureAllocInfo* p_textureAllocInfo);
	void Texture_alloc(Texture** p_texture, TextureAllocInfo* p_textureAllocInfo);
	void Texture_free(Texture* p_texture, RenderInterface* p_renderInterface);
	void Texture_free(Texture** p_texture, RenderInterface* p_renderInterface);

	VkImageSubresourceRange Texture_buildSubResource(Texture* p_texture);
}