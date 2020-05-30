#include "SwapChainImage.h"

#include "Render/Texture/Texture.h"

namespace _GameEngine::_Render
{
	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo)
	{
		p_swapChainImage->SwapChainImage = p_swapChainImageInitializationInfo->CreatedImage;

		ImageViewInitializationInfo l_imageViewInitializationInfo{};
		l_imageViewInitializationInfo.Texture = p_swapChainImage->SwapChainImage;
		l_imageViewInitializationInfo.TextureInfo = p_swapChainImageInitializationInfo->TextureInfo;
		l_imageViewInitializationInfo.Device = p_swapChainImageInitializationInfo->Device;
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