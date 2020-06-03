#include "Texture.h"

#include <stdexcept>

#include "stb_image.h"
#include "Log/Log.h"

#include "Render/VulkanObjects/Hardware/Device/Device.h"
#include "Render/VulkanObjects/Memory/VulkanBuffer.h"
#include "Render/LoopStep/PreRenderDeferedCommandBufferStep.h"
#include "Render/VulkanObjects/CommandBuffer/DeferredOperations/TextureLoadDeferredOperation.h"
#include "Render/Texture/InitializationConfigurations/TCColorShader.h"

namespace _GameEngine::_Render
{
	/// Validation
	void check_textureValidationToken_undefinedBehavior(Texture* l_texture, PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep);
	///

	void texture_AllocateVulkanObjects(Texture* p_texture,
		uint32_t p_width, uint32_t p_height,
		VkImageCreateInfoProvider p_imageCreateInfoProvider,
		ImageViewCreationInfoProvider imageViewCreationProvider,
		Device* p_device);

	Texture* Texture_loadFromFile(TextureLoadInfo* l_textureLoadInfo)
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

		texture_AllocateVulkanObjects(l_texture,
			static_cast<uint32_t>(l_texWidth),
			static_cast<uint32_t>(l_texHeight),
			TCColorShader_BuildVkImageCreateInfo,
			TCColorShader_BuildVkImageViewCreateInfo,
			l_textureLoadInfo->Device
		);

		BufferAllocInfo l_stagingBufferAllocInfo{};
		l_stagingBufferAllocInfo.Size = l_imageSize;
		l_stagingBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		l_stagingBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;


		VulkanBuffer l_stagingBuffer{};
		VulkanBuffer_alloc(&l_stagingBuffer, &l_stagingBufferAllocInfo, l_textureLoadInfo->Device);
		VulkanBuffer_pushToGPU(&l_stagingBuffer, l_textureLoadInfo->Device, l_pixels, l_imageSize);


		TextureLoadDeferredOperation* l_textureDeferredOperation = new TextureLoadDeferredOperation();
		l_textureDeferredOperation->Device = l_textureLoadInfo->Device;
		l_textureDeferredOperation->Texture = l_texture;
		l_textureDeferredOperation->SourceBuffer = l_stagingBuffer;
		DeferredCommandBufferOperation l_commandDeferredOperation = TextureLoadDeferredOperation_build(&l_textureDeferredOperation);
		SmartDeferredCommandBufferCompletionToken_build(&l_texture->TextureInitializationBufferCompletionToken, &l_commandDeferredOperation.DeferredCommandBufferCompletionToken);
		
		l_textureLoadInfo->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_commandDeferredOperation));


		stbi_image_free(l_pixels);

		return l_texture;
	};

	Texture* Texture_proceduralInstance(TextureProceduralInstanceInfo* p_textureProceduralInstanceInfo)
	{
		Texture* l_texture = new Texture();

		l_texture->TextureUniqueKey = *p_textureProceduralInstanceInfo->TextureKey;

		texture_AllocateVulkanObjects(l_texture,
			p_textureProceduralInstanceInfo->Width,
			p_textureProceduralInstanceInfo->Height,
			p_textureProceduralInstanceInfo->ImageCreateInfoProvider,
			p_textureProceduralInstanceInfo->ImageViewCreationInfoProvider,
			p_textureProceduralInstanceInfo->Device
		);

		DeferredCommandBufferOperation l_deferredCommandBufferOperation{};
		p_textureProceduralInstanceInfo->AllocDeferredCommandBufferOperation(&l_deferredCommandBufferOperation, l_texture);
		SmartDeferredCommandBufferCompletionToken_build(&l_texture->TextureInitializationBufferCompletionToken, &l_deferredCommandBufferOperation.DeferredCommandBufferCompletionToken);
		p_textureProceduralInstanceInfo->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_deferredCommandBufferOperation));

		return l_texture;
	};

	void Texture_free(Texture** p_texture, Device* p_device, PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep)
	{
		Texture* l_texture = *p_texture;
		ImageView_free(&l_texture->ImageView, p_device);
		vkDestroyImage(p_device->LogicalDevice.LogicalDevice, l_texture->Texture, nullptr);
		vkFreeMemory(p_device->LogicalDevice.LogicalDevice, l_texture->TextureMemory, nullptr);
		l_texture->Texture = VK_NULL_HANDLE;
		l_texture->TextureMemory = VK_NULL_HANDLE;

        check_textureValidationToken_undefinedBehavior(l_texture, p_preRenderDeferedCommandBufferStep);

		if (!SmartDeferredCommandBufferCompletionToken_isNull(&l_texture->TextureInitializationBufferCompletionToken))
		{
			l_texture->TextureInitializationBufferCompletionToken.TokenReference->IsCancelled = true;
		}

		delete l_texture;
		l_texture = nullptr;
	};

	void texture_AllocateVulkanObjects(Texture* p_texture,
		uint32_t p_width, uint32_t p_height,
		VkImageCreateInfoProvider p_imageCreateInfoProvider,
		ImageViewCreationInfoProvider imageViewCreationProvider,
		Device* p_device)
	{
		VkImageCreateInfo l_imageCreateInfo = p_imageCreateInfoProvider(p_width, p_height, p_device);

		if (vkCreateImage(p_device->LogicalDevice.LogicalDevice, &l_imageCreateInfo, nullptr, &p_texture->Texture) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create image!"));
		}

		p_texture->TextureInfo.Format = l_imageCreateInfo.format;
		p_texture->TextureInfo.Width = l_imageCreateInfo.extent.width;
		p_texture->TextureInfo.Height = l_imageCreateInfo.extent.height;
		p_texture->TextureInfo.Depth = l_imageCreateInfo.extent.depth;
		p_texture->TextureInfo.MipLevels = l_imageCreateInfo.mipLevels;
		p_texture->TextureInfo.ArrayLayers = l_imageCreateInfo.arrayLayers;

		VkMemoryRequirements l_memoryRequiremens;
		vkGetImageMemoryRequirements(p_device->LogicalDevice.LogicalDevice, p_texture->Texture, &l_memoryRequiremens);

		VkMemoryAllocateInfo l_textureMemoryAllocateInfo{};
		l_textureMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		l_textureMemoryAllocateInfo.allocationSize = l_memoryRequiremens.size;
		l_textureMemoryAllocateInfo.memoryTypeIndex = Device_findMemoryType(p_device, l_memoryRequiremens.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(p_device->LogicalDevice.LogicalDevice, &l_textureMemoryAllocateInfo, nullptr, &p_texture->TextureMemory) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to allocate image memory!"));
		}

		vkBindImageMemory(p_device->LogicalDevice.LogicalDevice, p_texture->Texture, p_texture->TextureMemory, 0);


		ImageViewInitializationInfo l_imageViewInitializationInfo{};
		l_imageViewInitializationInfo.Device = p_device;
		l_imageViewInitializationInfo.Texture = p_texture->Texture;
		l_imageViewInitializationInfo.TextureInfo = &p_texture->TextureInfo;
		l_imageViewInitializationInfo.ImageViewCreateInfoProvider = imageViewCreationProvider;
		ImageView_init(&p_texture->ImageView, &l_imageViewInitializationInfo);
	};




	void check_textureValidationToken_undefinedBehavior(_GameEngine::_Render::Texture* l_texture, _GameEngine::_Render::PreRenderDeferedCommandBufferStep* p_preRenderDeferedCommandBufferStep)
	{
#ifndef NDEBUG
		if (!SmartDeferredCommandBufferCompletionToken_isNull(&l_texture->TextureInitializationBufferCompletionToken))
		{
			bool l_tokenFoundInDeferrendOperations = false;
			for (auto&& l_defferedInitializationOperation : p_preRenderDeferedCommandBufferStep->DefferedOperations)
			{
				if (l_defferedInitializationOperation.DeferredCommandBufferCompletionToken == l_texture->TextureInitializationBufferCompletionToken.TokenReference)
				{
					l_tokenFoundInDeferrendOperations = true;
					break;
				}
			}

			if (!l_tokenFoundInDeferrendOperations)
			{
				_Log::LogInstance->CoreLogger->warn(
					"The local reference of DeferredCommandBufferCompletionToken is not null. However, it's reference is not found inside the PreRenderDeferedCommandBufferStep."
					" Because it is up to the PreRenderDeferedCommandBufferStep to dispose the token, this means that it's duplicated reference has not been cleared on completion. This may lead to undefined behavior."
					" Be sure that the token reference is nullified when destroyed."
				);
			}
		}
#endif
	}
}