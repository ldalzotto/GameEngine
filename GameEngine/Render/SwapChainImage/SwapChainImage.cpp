#include "SwapChainImage.h"

namespace _GameEngine::_Render::_SwapChainImage
{
	void SwapChainImage_init(SwapChainImage* p_swapChainImage, SwapChainImageInitializationInfo* p_swapChainImageInitializationInfo)
	{
		p_swapChainImage->SwapChainImage = p_swapChainImageInitializationInfo->CreatedImage;

		ImageViewInitializationInfo l_imageViewInitializationInfo{};
		l_imageViewInitializationInfo.SwapChainImage = p_swapChainImage->SwapChainImage;
		l_imageViewInitializationInfo.SwapChainInfo = p_swapChainImageInitializationInfo->SwapChainInfo;
		ImageViewsDependencies l_imageViewDependencies{};
		l_imageViewDependencies.Device = p_swapChainImageInitializationInfo->Device;
		l_imageViewInitializationInfo.ImageViewDependencies = &l_imageViewDependencies;
		ImageView_init(&p_swapChainImage->ImageView, &l_imageViewInitializationInfo);

		_CommandBuffer::CommandBuffersDependencies l_commandBufferDependencies{};
		l_commandBufferDependencies.CommandPool = p_swapChainImageInitializationInfo->CommandPool;
		_CommandBuffer::CommandBuffer_init(&p_swapChainImage->CommandBuffer, &l_commandBufferDependencies);
	};

	void SwapChainImage_free(SwapChainImage* p_swapChainImage)
	{
		ImageView_free(&p_swapChainImage->ImageView);
	};
};