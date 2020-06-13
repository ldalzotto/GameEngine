#include "SwapChainImage.h"

#include "Texture/Texture.h"

#include "RenderInterface.h"
#include "VulkanObjects/Hardware/Device/Device.h"
// #include "LoopStep/PreRenderDeferedCommandBufferStep.h"
// #include "VulkanObjects/CommandBuffer/DeferredOperations/SwapChainImageDeferredOperation.h"

namespace _GameEngine::_Render
{
	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo)
	{
		p_swapChainImage->SwapChainImage = p_swapChainImageInitializationInfo->CreatedImage;

		ImageViewInitializationInfo l_imageViewInitializationInfo{};
		l_imageViewInitializationInfo.Texture = p_swapChainImage->SwapChainImage;
		l_imageViewInitializationInfo.ImageViewCreateInfo = p_swapChainImageInitializationInfo->ImageViewCreateInfo;
		l_imageViewInitializationInfo.Device = p_swapChainImageInitializationInfo->RenderInterface->Device;

		p_swapChainImage->ImageViewCreateInfo = *p_swapChainImageInitializationInfo->ImageViewCreateInfo;
		ImageView_init(&p_swapChainImage->ImageView, &l_imageViewInitializationInfo);

		CommandBuffersDependencies l_commandBufferDependencies{};
		l_commandBufferDependencies.CommandPool = p_swapChainImageInitializationInfo->RenderInterface->CommandPool;
		CommandBuffer_init(&p_swapChainImage->CommandBuffer, &l_commandBufferDependencies);

		/*
		DeferredCommandBufferOperation l_swapChainImageInitializationOepration = SwapChainImageDeferredOperation_alloc(p_swapChainImage);
		SmartDeferredCommandBufferCompletionToken_build(&p_swapChainImage->SwapChainImageInitializationToken, &l_swapChainImageInitializationOepration.DeferredCommandBufferCompletionToken);

		p_swapChainImageInitializationInfo->RenderInterface->PreRenderDeferedCommandBufferStep->DefferedOperations.push_back(l_swapChainImageInitializationOepration);
		*/
	};

	void SwapChainImage_free(SwapChainImage* p_swapChainImage, Device* p_device)
	{
		/*
		if (!SmartDeferredCommandBufferCompletionToken_isNull(&p_swapChainImage->SwapChainImageInitializationToken))
		{
			if (!p_swapChainImage->SwapChainImageInitializationToken.TokenReference->IsCompleted)
			{
				p_swapChainImage->SwapChainImageInitializationToken.TokenReference->IsCancelled = true;
			}
		}
		*/
		ImageView_free(&p_swapChainImage->ImageView, p_device);
	};

	VkImageSubresourceRange SwapChainImage_buildSubResource(SwapChainImage* p_swapChainImage)
	{
		VkImageSubresourceRange l_subResourceRange{};
		l_subResourceRange.aspectMask = p_swapChainImage->ImageViewCreateInfo.aspectMask;
		l_subResourceRange.baseMipLevel = p_swapChainImage->ImageViewCreateInfo.MipLevels - 1;
		l_subResourceRange.layerCount = p_swapChainImage->ImageViewCreateInfo.MipLevels;
		l_subResourceRange.baseArrayLayer = p_swapChainImage->ImageViewCreateInfo.ArrayLayers - 1;
		l_subResourceRange.levelCount = p_swapChainImage->ImageViewCreateInfo.ArrayLayers;
		return l_subResourceRange;
	};
};