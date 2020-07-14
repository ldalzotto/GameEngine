#include "Texture.h"

#include <stdexcept>
#include "stb_image.h"

#include "RenderInterface.h"
#include "TextureSwapChainSizeSynchronizer.h"

#include "Log/Log.hpp"
#include "Functional/ToString/ToString.hpp"

extern "C"
{
#include "Functional/Hash/Hash.h"
}

#include "VulkanObjects/SwapChain/SwapChain.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include "VulkanObjects/Memory/VulkanBuffer.h"
#include "RenderStep/PreRenderDeferedCommandBufferStep.h"
#include "VulkanObjects/CommandBuffer/DeferredOperations/TextureLoadDeferredOperation.h"
#include "Texture/InitializationConfigurations/TCColorShader.h"
#include "Texture/InitializationConfigurations/TCDepth.h"

namespace _GameEngine::_Render
{
	//////////////////// Texture instanciation ////////////////////
	void Texture_loadFromFile(Texture* p_texture, TextureAllocInfo* p_textureAllocInfo);
	void Texture_proceduralInstance(Texture* p_texture, TextureAllocInfo* p_textureAllocInfo);
	///////////////////////////////////////////////////////////////

	//////////////////// Validation ////////////////////
	void check_textureValidationToken_undefinedBehavior(Texture* l_texture, RenderInterface* p_renderInterface);
	////////////////////////////////////////////////////

	//////////////////// Fit swap chain size feature ////////////////////
	void texture_enableFitSwapChainSizeFeature(Texture* p_texture, TextureAllocInfo* p_textureAllocInfo);
	void texture_removeFitSwapChainSizeFeature(Texture* p_texture, RenderInterface* p_renderInterface);
	/////////////////////////////////////////////////////////////////////

	//////////////////// Texture internal object creation ////////////////////
	void texture_buildCreationInfoObjects(TextureCreateInfo* p_textureCreateInfo, RenderInterface* p_renderInterface, TextureInfo* out_textureInfo, ImageViewCreateInfo* out_imageViewCreateInfo);
	void texture_buildDeferredInitializationOperation(TextureCreateInfo* p_textureCreateInfo, Texture* p_texture, RenderInterface* p_renderInterface, VkDeviceSize p_imageSize, stbi_uc* p_pixels);
	void texture_buildDeferredInitializationOperation(TextureCreateInfo* p_textureCreateInfo, Texture* p_texture, RenderInterface* p_renderInterface);
	void texture_AllocateVulkanObjects(Texture* p_texture, ImageViewCreateInfo* p_imageViewCreateInfo, Device* p_device);
	//////////////////////////////////////////////////////////////////////////

	Texture* Texture_alloc(TextureAllocInfo* p_textureAllocInfo)
	{
		Texture* l_texture = new Texture();
		Texture_alloc(&l_texture, p_textureAllocInfo);
		return l_texture;
	};

	void Texture_alloc(Texture** p_texture, TextureAllocInfo* p_textureAllocInfo)
	{
		Texture* l_texture = *p_texture;
		l_texture->TextureUniqueKey = p_textureAllocInfo->TextureKey;

		texture_enableFitSwapChainSizeFeature(l_texture, p_textureAllocInfo);

		if (p_textureAllocInfo->TextureAllocationType == TextureAllocationType::FILE)
		{
			Texture_loadFromFile(l_texture, p_textureAllocInfo);
		}
		else if (p_textureAllocInfo->TextureAllocationType == TextureAllocationType::PROCEDURAL)
		{
			Texture_proceduralInstance(l_texture, p_textureAllocInfo);
		}
		else
		{
			delete l_texture;
			{
				_Core::String l_errorMessage; _Core::String_alloc(&l_errorMessage, 100);
				_Core::String_append(&l_errorMessage, "TextureAllocation : the TextureAllocationType ");
				_Core::ToString_int(&l_errorMessage, (int*)p_textureAllocInfo->TextureAllocationType);
				_Core::String_append(&l_errorMessage, " is not supported.");
				throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE_STRING(&l_errorMessage));
			}
		}
	};

	void Texture_loadFromFile(Texture* p_texture, TextureAllocInfo* p_textureAllocInfo)
	{
		ImageViewCreateInfo l_imageViewCreateInfo{};
		texture_buildCreationInfoObjects(&p_textureAllocInfo->TextureCreateInfo, p_textureAllocInfo->RenderInterface, &p_texture->TextureInfo, &l_imageViewCreateInfo);

		int l_texWidth, l_texHeight, l_texChannels;
		stbi_uc* l_pixels = stbi_load(p_texture->TextureUniqueKey.TexturePath.data(), &l_texWidth, &l_texHeight, &l_texChannels, STBI_rgb_alpha);
		VkDeviceSize l_imageSize = l_texWidth * l_texHeight * (STBI_rgb_alpha);

		if (!l_pixels)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to load texture image!"));
		}

		{
			p_texture->TextureInfo.Width = static_cast<uint32_t>(l_texWidth);
			p_texture->TextureInfo.Height = static_cast<uint32_t>(l_texHeight);

			texture_AllocateVulkanObjects(p_texture,
				&l_imageViewCreateInfo,
				p_textureAllocInfo->RenderInterface->Device
			);
		}
		{
			texture_buildDeferredInitializationOperation(&p_textureAllocInfo->TextureCreateInfo, p_texture, p_textureAllocInfo->RenderInterface, l_imageSize, l_pixels);
		}

		stbi_image_free(l_pixels);
	};

	void Texture_proceduralInstance(Texture* p_texture, TextureAllocInfo* p_textureAllocInfo)
	{
		ImageViewCreateInfo l_imageViewCreateInfo{};
		texture_buildCreationInfoObjects(&p_textureAllocInfo->TextureCreateInfo, p_textureAllocInfo->RenderInterface, &p_texture->TextureInfo, &l_imageViewCreateInfo);


		texture_AllocateVulkanObjects(p_texture,
			&l_imageViewCreateInfo,
			p_textureAllocInfo->RenderInterface->Device
		);

		texture_buildDeferredInitializationOperation(&p_textureAllocInfo->TextureCreateInfo, p_texture, p_textureAllocInfo->RenderInterface);
	};

	void Texture_free(Texture* p_texture, RenderInterface* p_renderInterface)
	{
		ImageView_free(&p_texture->ImageView, p_renderInterface->Device);
		vkDestroyImage(p_renderInterface->Device->LogicalDevice.LogicalDevice, p_texture->Texture, nullptr);
		vkFreeMemory(p_renderInterface->Device->LogicalDevice.LogicalDevice, p_texture->TextureMemory, nullptr);
		p_texture->Texture = VK_NULL_HANDLE;
		p_texture->TextureMemory = VK_NULL_HANDLE;

		texture_removeFitSwapChainSizeFeature(p_texture, p_renderInterface);

		check_textureValidationToken_undefinedBehavior(p_texture, p_renderInterface);

		if (!SmartDeferredCommandBufferCompletionToken_isNull(&p_texture->TextureInitializationBufferCompletionToken))
		{
			p_texture->TextureInitializationBufferCompletionToken.TokenReference->IsCancelled = true;
		}
	};

	void Texture_free(Texture** p_texture, RenderInterface* p_renderInterface)
	{
		Texture* l_texture = *p_texture;
		Texture_free(l_texture, p_renderInterface);
		delete l_texture;
	};

	VkImageSubresourceRange Texture_buildSubResource(Texture* p_texture)
	{
		VkImageSubresourceRange l_subResourceRange{};
		l_subResourceRange.aspectMask = p_texture->TextureInfo.AspectMask;
		l_subResourceRange.baseMipLevel = p_texture->TextureInfo.MipLevels - 1;
		l_subResourceRange.layerCount = p_texture->TextureInfo.MipLevels;
		l_subResourceRange.baseArrayLayer = p_texture->TextureInfo.ArrayLayers - 1;
		l_subResourceRange.levelCount = p_texture->TextureInfo.ArrayLayers;
		return l_subResourceRange;
	};

	void texture_AllocateVulkanObjects(Texture* p_texture, ImageViewCreateInfo* p_imageViewCreateInfo, Device* p_device)
	{
		VkImageCreateInfo l_imageCreateInfo{};

		{
			l_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			l_imageCreateInfo.imageType = p_texture->TextureInfo.ImageType;
			l_imageCreateInfo.extent.width = p_texture->TextureInfo.Width;
			l_imageCreateInfo.extent.height = p_texture->TextureInfo.Height;
			l_imageCreateInfo.extent.depth = 1;
			l_imageCreateInfo.mipLevels = p_texture->TextureInfo.MipLevels;
			l_imageCreateInfo.arrayLayers = p_texture->TextureInfo.ArrayLayers;
			l_imageCreateInfo.format = p_texture->TextureInfo.Format;
			l_imageCreateInfo.tiling = p_texture->TextureInfo.Tiling;
			l_imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			l_imageCreateInfo.usage = p_texture->TextureInfo.Usage;
			l_imageCreateInfo.sharingMode = p_texture->TextureInfo.SharingMode;
			l_imageCreateInfo.samples = p_texture->TextureInfo.Samples;
		}

		if (vkCreateImage(p_device->LogicalDevice.LogicalDevice, &l_imageCreateInfo, nullptr, &p_texture->Texture) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create image!"));
		}

		{
			p_imageViewCreateInfo->aspectMask = p_texture->TextureInfo.AspectMask;
			p_imageViewCreateInfo->Format = p_texture->TextureInfo.Format;
			p_imageViewCreateInfo->MipLevels = p_texture->TextureInfo.MipLevels;
			p_imageViewCreateInfo->ArrayLayers = p_texture->TextureInfo.ArrayLayers;
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
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to allocate image memory!"));
		}

		vkBindImageMemory(p_device->LogicalDevice.LogicalDevice, p_texture->Texture, p_texture->TextureMemory, 0);

		ImageViewInitializationInfo l_imageViewInitializationInfo{};
		l_imageViewInitializationInfo.Device = p_device;
		l_imageViewInitializationInfo.Texture = p_texture->Texture;
		l_imageViewInitializationInfo.ImageViewCreateInfo = p_imageViewCreateInfo;
		ImageView_init(&p_texture->ImageView, &l_imageViewInitializationInfo);
	};

	void check_textureValidationToken_undefinedBehavior(_GameEngine::_Render::Texture* l_texture, _GameEngine::_Render::RenderInterface* p_renderInterface)
	{
#ifndef NDEBUG
		if (!SmartDeferredCommandBufferCompletionToken_isNull(&l_texture->TextureInitializationBufferCompletionToken))
		{
			bool l_tokenFoundInDeferrendOperations = false;
			for (auto&& l_defferedInitializationOperation : p_renderInterface->PreRenderDeferedCommandBufferStep->DefferedOperations)
			{
				if (l_defferedInitializationOperation.DeferredCommandBufferCompletionToken == l_texture->TextureInitializationBufferCompletionToken.TokenReference)
				{
					l_tokenFoundInDeferrendOperations = true;
					break;
				}
			}

			if (!l_tokenFoundInDeferrendOperations)
			{
				MYLOG_PUSH(p_renderInterface->MyLog, _Core::LogLevel::WARN ,
					"The local reference of DeferredCommandBufferCompletionToken is not null. However, it's reference is not found inside the PreRenderDeferedCommandBufferStep."
					" Because it is up to the PreRenderDeferedCommandBufferStep to dispose the token, this means that it's duplicated reference has not been cleared on completion. This may lead to undefined behavior."
					" Be sure that the token reference is nullified when destroyed.");
			}
		}
#endif
	}

	void texture_buildCreationInfoObjects(TextureCreateInfo* p_textureCreateInfo, RenderInterface* p_renderInterface, TextureInfo* out_textureInfo, ImageViewCreateInfo* out_imageViewCreateInfo)
	{
		out_textureInfo->Width = p_textureCreateInfo->Width;
		out_textureInfo->Height = p_textureCreateInfo->Height;

		if (p_textureCreateInfo->TextureType == TextureType::COLOR
			&& p_textureCreateInfo->TextureUsage == TextureUsage::SHADER_INPUT)
		{
			TCColorShader_BuildTextureInfo(out_textureInfo);
			TCColorShader_BuildVkImageViewCreateInfo(out_imageViewCreateInfo);
		}
		else if (p_textureCreateInfo->TextureType == TextureType::DEPTH
			&& p_textureCreateInfo->TextureUsage == TextureUsage::PIPELINE_ATTACHMENT)
		{
			TCDepth_BuildTextureInfo(out_textureInfo, p_renderInterface->Device);
			TCDepth_BuildVkImageViewCreateInfo(out_imageViewCreateInfo);
		}
		else
		{
			_Core::String l_errorMessage; _Core::String_alloc(&l_errorMessage, 100);
			_Core::String_append(&l_errorMessage, "TextureBuildCreationInfoObject : Texture build creation object with TextureType : ");
			_Core::ToString_int(&l_errorMessage, (int*)p_textureCreateInfo->TextureType);
			_Core::String_append(&l_errorMessage, "and TextureUsage : ");
			_Core::ToString_int(&l_errorMessage, (int*)p_textureCreateInfo->TextureUsage);
			_Core::String_append(&l_errorMessage, " is not supported.");
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE_STRING(&l_errorMessage));
		}
	};

	void texture_buildDeferredInitializationOperation(TextureCreateInfo* p_textureCreateInfo, Texture* p_texture, RenderInterface* p_renderInterface, VkDeviceSize p_imageSize, stbi_uc* p_pixels)
	{
		if (p_textureCreateInfo->TextureType == TextureType::COLOR
			&& p_textureCreateInfo->TextureUsage == TextureUsage::SHADER_INPUT)
		{

			BufferAllocInfo l_stagingBufferAllocInfo{};
			l_stagingBufferAllocInfo.Size = p_imageSize;
			l_stagingBufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			l_stagingBufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;


			VulkanBuffer l_stagingBuffer{};
			VulkanBuffer_alloc(&l_stagingBuffer, &l_stagingBufferAllocInfo, p_renderInterface->Device);
			VulkanBuffer_pushToGPU(&l_stagingBuffer, p_renderInterface->Device, p_pixels, p_imageSize);


			TextureLoadDeferredOperation* l_textureDeferredOperation = new TextureLoadDeferredOperation();
			l_textureDeferredOperation->Device = p_renderInterface->Device;
			l_textureDeferredOperation->Texture = p_texture;
			l_textureDeferredOperation->SourceBuffer = l_stagingBuffer;
			DeferredCommandBufferOperation l_commandDeferredOperation = TextureLoadDeferredOperation_build(&l_textureDeferredOperation);
			SmartDeferredCommandBufferCompletionToken_build(&p_texture->TextureInitializationBufferCompletionToken, &l_commandDeferredOperation.DeferredCommandBufferCompletionToken);

			p_renderInterface->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_commandDeferredOperation));

		}
		else if (p_textureCreateInfo->TextureType == TextureType::DEPTH
			&& p_textureCreateInfo->TextureUsage == TextureUsage::PIPELINE_ATTACHMENT)
		{
			_Core::String l_errorMessage; _Core::String_alloc(&l_errorMessage, 100);
			_Core::String_append(&l_errorMessage, "TextureDeferredOperation : Texture build creation object with TextureType : ");
			_Core::ToString_int(&l_errorMessage, (int*)p_textureCreateInfo->TextureType);
			_Core::String_append(&l_errorMessage, "and TextureUsage : ");
			_Core::ToString_int(&l_errorMessage, (int*)p_textureCreateInfo->TextureUsage);
			_Core::String_append(&l_errorMessage, " is not supported.");
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE_STRING(&l_errorMessage));
		}
	};

	void texture_buildDeferredInitializationOperation(TextureCreateInfo* p_textureCreateInfo, Texture* p_texture, RenderInterface* p_renderInterface)
	{
		if (p_textureCreateInfo->TextureType == TextureType::DEPTH
			&& p_textureCreateInfo->TextureUsage == TextureUsage::PIPELINE_ATTACHMENT)
		{
			DeferredCommandBufferOperation l_deferredCommandBufferOperation{};
			TCDepth_InitializationCommandBufferOperation_build(&l_deferredCommandBufferOperation, p_texture);
			p_renderInterface->PreRenderDeferedCommandBufferStep->DefferedOperations.emplace_back(std::move(l_deferredCommandBufferOperation));
		}
		else
		{
			_Core::String l_errorMessage; _Core::String_alloc(&l_errorMessage, 100);
			_Core::String_append(&l_errorMessage, "TextureDeferredOperation : Texture build creation object with TextureType : ");
			_Core::ToString_int(&l_errorMessage, (int*)p_textureCreateInfo->TextureType);
			_Core::String_append(&l_errorMessage, "and TextureUsage : ");
			_Core::ToString_int(&l_errorMessage, (int*)p_textureCreateInfo->TextureUsage);
			_Core::String_append(&l_errorMessage, " is not supported.");
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE_STRING(&l_errorMessage));
		}
	};

	void texture_enableFitSwapChainSizeFeature(Texture* p_texture, TextureAllocInfo* p_textureAllocInfo)
	{
		if (p_textureAllocInfo->FitSwapChainSize)
		{
#ifndef NDEBUG
			if (p_textureAllocInfo->TextureAllocationType == TextureAllocationType::FILE)
			{
				delete p_texture;
				throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("TextureAllocation : cannot activate the FIT_SWAPCHAIN_SIZE feature when allocation type is TextureAllocationType::FILE."));
			}
#endif
			p_textureAllocInfo->TextureCreateInfo.Width = p_textureAllocInfo->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.width;
			p_textureAllocInfo->TextureCreateInfo.Height = p_textureAllocInfo->RenderInterface->SwapChain->SwapChainInfo.SwapExtend.height;
			p_texture->Features.FitSwapChainSizeEnabled = true;
			TextureSwapChainSizeSynchronizer_addSynchronizer(p_textureAllocInfo->RenderInterface->TextureSwapChainSizeSynchronizer, p_texture, p_textureAllocInfo);
		}
	};

	void texture_removeFitSwapChainSizeFeature(Texture* p_texture, RenderInterface* p_renderInterface)
	{
		if (p_texture->Features.FitSwapChainSizeEnabled)
		{
			p_texture->Features.FitSwapChainSizeEnabled = false;
			TextureSwapChainSizeSynchronizer_removeSynchronizer(p_renderInterface->TextureSwapChainSizeSynchronizer, p_texture);
		}
	}

	size_t TextureUniqueKey_Hash(TextureUniqueKey* p_key)
	{
		return Core_HashCombine_string(0, (char*)p_key->TexturePath.c_str());
	};
}
