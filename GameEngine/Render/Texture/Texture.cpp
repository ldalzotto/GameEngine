#include "Texture.h"

#include <stdexcept>

#include "stb_image.h"
#include "Log/Log.h"

#include "Render/Hardware/Device/Device.h"
#include "Render/Memory/VulkanBuffer.h"
#include "Render/LoopStep/PreRenderDeferedCommandBufferStep.h"
#include "Render/CommandBuffer/DeferredOperations/TextureLoadDeferredOperation.h"

namespace _GameEngine::_Render
{

	Texture* Texture_alloc(TextureLoadInfo* l_textureLoadInfo)
	{
		Texture* l_texture = new Texture();

		l_texture->TextureUniqueKey = *l_textureLoadInfo->TextureKey;

		int l_texWidth, l_texHeight, l_texChannels;
		stbi_uc* l_pixels = stbi_load(l_texture->TextureUniqueKey.TexturePath.data(), &l_texWidth, &l_texHeight, &l_texChannels, STBI_rgb_alpha);
		VkDeviceSize l_imageSize = l_texWidth * l_texHeight * (STBI_rgb_alpha);

		if (!l_pixels)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to load texture image!"));
		}

		BufferAllocInfo l_stagingBufferAllocInfo{};
		l_stagingBufferAllocInfo.Size = l_imageSize;
		l_stagingBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		l_stagingBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		VulkanBuffer l_stagingBuffer{};

		VulkanBuffer_alloc(&l_stagingBuffer, &l_stagingBufferAllocInfo, l_textureLoadInfo->Device);
		VulkanBuffer_pushToGPU(&l_stagingBuffer, l_textureLoadInfo->Device, l_pixels, l_imageSize);
		stbi_image_free(l_pixels);

		VkImageCreateInfo l_imageCreateInfo{};
		l_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		l_imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		l_imageCreateInfo.extent.width = static_cast<uint32_t>(l_texWidth);
		l_imageCreateInfo.extent.height = static_cast<uint32_t>(l_texHeight);
		l_imageCreateInfo.extent.depth = 1;
		l_imageCreateInfo.mipLevels = 1;
		l_imageCreateInfo.arrayLayers = 1;
		l_imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		l_imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		l_imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		l_imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		l_imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		l_imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		l_imageCreateInfo.flags = 0;

		if (vkCreateImage(l_textureLoadInfo->Device->LogicalDevice.LogicalDevice, &l_imageCreateInfo, nullptr, &l_texture->Texture) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create image!"));
		}

		l_texture->TextureInfo.Format = l_imageCreateInfo.format;
		l_texture->TextureInfo.Width = l_imageCreateInfo.extent.width;
		l_texture->TextureInfo.Height = l_imageCreateInfo.extent.height;
		l_texture->TextureInfo.Depth = l_imageCreateInfo.extent.depth;
		l_texture->TextureInfo.MipLevels = l_imageCreateInfo.mipLevels;
		l_texture->TextureInfo.ArrayLayers = l_imageCreateInfo.arrayLayers;

		VkMemoryRequirements l_memoryRequiremens;
		vkGetImageMemoryRequirements(l_textureLoadInfo->Device->LogicalDevice.LogicalDevice, l_texture->Texture, &l_memoryRequiremens);

		VkMemoryAllocateInfo l_textureMemoryAllocateInfo{};
		l_textureMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		l_textureMemoryAllocateInfo.allocationSize = l_memoryRequiremens.size;
		l_textureMemoryAllocateInfo.memoryTypeIndex = Device_findMemoryType(l_textureLoadInfo->Device, l_memoryRequiremens.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(l_textureLoadInfo->Device->LogicalDevice.LogicalDevice, &l_textureMemoryAllocateInfo, nullptr, &l_texture->TextureMemory) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to allocate image memory!"));
		}

		vkBindImageMemory(l_textureLoadInfo->Device->LogicalDevice.LogicalDevice, l_texture->Texture, l_texture->TextureMemory, 0);


		ImageViewInitializationInfo l_imageViewInitializationInfo{};
		l_imageViewInitializationInfo.Device = l_textureLoadInfo->Device;
		l_imageViewInitializationInfo.Texture = l_texture->Texture;
		l_imageViewInitializationInfo.TextureInfo = &l_texture->TextureInfo;
		ImageView_init(&l_texture->ImageView, &l_imageViewInitializationInfo);


		l_texture->TextureInitializationBufferCompletionToken = new DeferredCommandBufferCompletionToken();
		TextureLoadDeferredOperation* l_textureDeferredOperation = new TextureLoadDeferredOperation();
		l_textureDeferredOperation->Device = l_textureLoadInfo->Device;
		l_textureDeferredOperation->Texture = l_texture;
		l_textureDeferredOperation->SourceBuffer = l_stagingBuffer;
		DeferredCommandBufferOperation l_commandDeferredOperation = TextureLoadDeferredOperation_build(&l_textureDeferredOperation, &l_texture->TextureInitializationBufferCompletionToken);
		l_textureLoadInfo->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_commandDeferredOperation));

		return l_texture;
	};

	void Texture_free(Texture** p_texture, Device* p_device)
	{
		Texture* l_texture = *p_texture;
		ImageView_free(&l_texture->ImageView, p_device);
		vkDestroyImage(p_device->LogicalDevice.LogicalDevice, l_texture->Texture, nullptr);
		vkFreeMemory(p_device->LogicalDevice.LogicalDevice, l_texture->TextureMemory, nullptr);
		l_texture->Texture = VK_NULL_HANDLE;
		l_texture->TextureMemory = VK_NULL_HANDLE;
		l_texture->TextureInitializationBufferCompletionToken->IsCancelled = true;
		delete l_texture;
		l_texture = nullptr;
	};
}