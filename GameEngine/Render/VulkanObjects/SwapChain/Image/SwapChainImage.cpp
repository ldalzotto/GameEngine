#include "SwapChainImage.h"

#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo)
	{
		p_swapChainImage->SwapChainImage = p_swapChainImageInitializationInfo->CreatedImage;

		ImageViewCreateInfo l_imageViewCreateInfo{};
		l_imageViewCreateInfo.Format = VK_FORMAT_R8G8B8A8_SRGB;
		l_imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		l_imageViewCreateInfo.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		l_imageViewCreateInfo.MipLevels = 1;
		l_imageViewCreateInfo.ArrayLayers = 1;

		ImageViewInitializationInfo l_imageViewInitializationInfo{};
		l_imageViewInitializationInfo.Texture = p_swapChainImage->SwapChainImage;
		l_imageViewInitializationInfo.ImageViewCreateInfo = p_swapChainImageInitializationInfo->ImageViewCreateInfo;
		l_imageViewInitializationInfo.Device = p_swapChainImageInitializationInfo->Device;
		l_imageViewInitializationInfo.ImageViewCreateInfo = &l_imageViewCreateInfo;

		ImageView_init(&p_swapChainImage->ImageView, &l_imageViewInitializationInfo);

		CommandBuffersDependencies l_commandBufferDependencies{};
		l_commandBufferDependencies.CommandPool = p_swapChainImageInitializationInfo->CommandPool;
		CommandBuffer_init(&p_swapChainImage->CommandBuffer, &l_commandBufferDependencies);
	};

	void SwapChainImage_free(SwapChainImage* p_swapChainImage, Device* p_device)
	{
		ImageView_free(&p_swapChainImage->ImageView, p_device);
	};
};