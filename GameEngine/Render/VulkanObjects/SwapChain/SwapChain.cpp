#include "SwapChain.h"

#include <stdexcept>
#include <algorithm>

#include "Log/LogFormatting.hpp"

#include "RenderInterface.h"
#include "VulkanObjects/Hardware/Device/Device.h"
#include "VulkanObjects/Hardware/Window/Window.h"
#include "VulkanObjects/Hardware/Window/Surface.h"
#include "Texture/ImageViews.h"

namespace _GameEngine::_Render
{
	void swapChainInvalidate(SwapChain* p_swapChain);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> p_surfaceFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& p_availablePresentModes);
	VkExtent2D chooseSwapExtent(Window* p_window, const VkSurfaceCapabilitiesKHR& p_capabilities);
	void onWindowSizeChanged(SwapChain* p_swapChain, void* p_input);

	SwapChainSupportDetails SwapChain_getSupportDetails(VkPhysicalDevice p_physicalDevice, Surface* p_surface)
	{
		SwapChainSupportDetails l_swapChainSupportDetails{};
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_physicalDevice, p_surface->WindowSurface, &l_swapChainSupportDetails.Capabilities);

		uint32_t l_deviceSurfaceFormatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(p_physicalDevice, p_surface->WindowSurface, &l_deviceSurfaceFormatCount, nullptr);

		if (l_deviceSurfaceFormatCount > 0)
		{
			l_swapChainSupportDetails.SurfaceFormats = std::vector<VkSurfaceFormatKHR>(l_deviceSurfaceFormatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(p_physicalDevice, p_surface->WindowSurface, &l_deviceSurfaceFormatCount, l_swapChainSupportDetails.SurfaceFormats.data());
		}

		uint32_t l_presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(p_physicalDevice, p_surface->WindowSurface, &l_presentModeCount, nullptr);
		if (l_presentModeCount > 0)
		{
			l_swapChainSupportDetails.PresentModes = std::vector<VkPresentModeKHR>(l_presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(p_physicalDevice, p_surface->WindowSurface, &l_presentModeCount, l_swapChainSupportDetails.PresentModes.data());
		}

		return l_swapChainSupportDetails;
	};

	bool SwapChain_isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails)
	{
		return p_swapCahinSupportDetails.PresentModes.size() > 0 && p_swapCahinSupportDetails.SurfaceFormats.size() > 0;
	};

	void SwapChain_build(SwapChain* p_swapChain, RenderInterface* p_renderInterface)
	{
		// See SwapChain_free for a reason of why this condition exists.
		if (!p_swapChain->MustBeRebuilt)
		{
			_Core::ObserverT_alloc(&p_swapChain->OnSwapChainBuilded);
		}

		p_swapChain->RenderInterface = p_renderInterface;
		p_swapChain->OnWindowSizeChangeCallback = { onWindowSizeChanged, p_swapChain };
		_Core::ObserverT_register(&p_renderInterface->Window->OnWindowSizeChanged, (_Core::CallbackT<void, void>*) &p_swapChain->OnWindowSizeChangeCallback);

		SwapChainInfo* l_swapChainInfo = &p_swapChain->SwapChainInfo;

		SwapChainSupportDetails l_swapChainDetails = SwapChain_getSupportDetails(
			p_renderInterface->Device->PhysicalDevice.PhysicalDevice,
			p_renderInterface->WindowSurface);

		l_swapChainInfo->SurfaceFormat = chooseSwapSurfaceFormat(l_swapChainDetails.SurfaceFormats);
		l_swapChainInfo->PresentMode = chooseSwapPresentMode(l_swapChainDetails.PresentModes);
		l_swapChainInfo->SwapExtend = chooseSwapExtent(p_renderInterface->Window, l_swapChainDetails.Capabilities);

		uint32_t l_imageCount = l_swapChainDetails.Capabilities.minImageCount;
		if (l_swapChainDetails.Capabilities.minImageCount > 0 && l_imageCount > l_swapChainDetails.Capabilities.maxImageCount)
		{
			l_imageCount = l_swapChainDetails.Capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR l_createInfo{ };
		l_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		l_createInfo.surface = p_renderInterface->WindowSurface->WindowSurface;
		l_createInfo.minImageCount = l_imageCount;
		l_createInfo.imageFormat = l_swapChainInfo->SurfaceFormat.format;
		l_createInfo.imageColorSpace = l_swapChainInfo->SurfaceFormat.colorSpace;
		l_createInfo.imageExtent = l_swapChainInfo->SwapExtend;
		l_createInfo.imageArrayLayers = 1;
		l_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		
		// How swap chain is handling images
		auto l_physicalDeviceQueues = &p_renderInterface->Device->PhysicalDevice.QueueFamilies;

		// Graphics and Presentation queues are not the same.
		if (l_physicalDeviceQueues->Graphics.QueueIndex != l_physicalDeviceQueues->Present.QueueIndex)
		{
			l_createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			l_createInfo.queueFamilyIndexCount = 2;
			l_createInfo.pQueueFamilyIndices = new uint32_t[2]{ l_physicalDeviceQueues->Graphics.QueueIndex , l_physicalDeviceQueues->Present.QueueIndex };
		}
		else
		{
			l_createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			l_createInfo.queueFamilyIndexCount = 0;
			l_createInfo.pQueueFamilyIndices = nullptr;
		}

		l_createInfo.preTransform = l_swapChainDetails.Capabilities.currentTransform;
		l_createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		l_createInfo.presentMode = l_swapChainInfo->PresentMode;
		l_createInfo.clipped = VK_TRUE;
		l_createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(p_renderInterface->Device->LogicalDevice.LogicalDevice, &l_createInfo, nullptr, &p_swapChain->VkSwapchainKHR) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create swap chain!"));
		}


		std::vector<VkImage> l_swapChainRawImages;
		l_imageCount = 0;
		vkGetSwapchainImagesKHR(p_renderInterface->Device->LogicalDevice.LogicalDevice, p_swapChain->VkSwapchainKHR, &l_imageCount, nullptr);
		l_swapChainRawImages.resize(l_imageCount);
		vkGetSwapchainImagesKHR(p_renderInterface->Device->LogicalDevice.LogicalDevice, p_swapChain->VkSwapchainKHR, &l_imageCount, l_swapChainRawImages.data());
		
		p_swapChain->SwapChainImages.resize(l_imageCount);

		ImageViewCreateInfo l_imageViewCreateInfo{};
		l_imageViewCreateInfo.Format = p_swapChain->SwapChainInfo.SurfaceFormat.format;
		l_imageViewCreateInfo.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		l_imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		l_imageViewCreateInfo.MipLevels = 1;
		l_imageViewCreateInfo.ArrayLayers = 1;

		for (size_t i = 0; i < l_imageCount; i++)
		{
			SwapChainImageInitializationInfo l_swapChainImageInitializationInfo{};
			l_swapChainImageInitializationInfo.CreatedImage = l_swapChainRawImages[i];
			l_swapChainImageInitializationInfo.ImageViewCreateInfo = &l_imageViewCreateInfo;
			l_swapChainImageInitializationInfo.RenderInterface = p_renderInterface;

			SwapChainImage_init(&p_swapChain->SwapChainImages[i], &l_swapChainImageInitializationInfo);
		}
	};

	void SwapChain_broadcastRebuildEvent(SwapChain* p_swapChain, RenderInterface* p_renderInterface)
	{
		p_swapChain->MustBeRebuilt = false;
		_Core::ObserverT_broadcast(&p_swapChain->OnSwapChainBuilded, p_renderInterface);
	};

	void SwapChain_free(SwapChain* p_swapChain)
	{
		// We are making a distinction between when the swap chain is rebuilded or the application is ending.
		// When the swap chain is rebuiliding, we don't want to destroy external callback registered because they may persist even if the swap chain is rebuilded. (Example, the Editor rebuild pipeline based on this event).
		// Thus, we don't destroy it.
		if (!p_swapChain->MustBeRebuilt)
		{
			_Core::ObserverT_free(&p_swapChain->OnSwapChainBuilded);
		}
		_Core::ObserverT_unRegister(&p_swapChain->RenderInterface->Window->OnWindowSizeChanged, (_Core::CallbackT<void, void>*) &p_swapChain->OnWindowSizeChangeCallback);

		for (size_t i = 0; i < p_swapChain->SwapChainImages.size(); i++)
		{
			SwapChainImage_free(&p_swapChain->SwapChainImages[i], p_swapChain->RenderInterface->Device);
		}

		vkDestroySwapchainKHR(p_swapChain->RenderInterface->Device->LogicalDevice.LogicalDevice, p_swapChain->VkSwapchainKHR, nullptr);
	};

	void swapChainInvalidate(SwapChain* p_swapChain)
	{
		p_swapChain->MustBeRebuilt = true;
	};

	void onWindowSizeChanged(SwapChain* p_swapChain, void* p_input)
	{
        swapChainInvalidate(p_swapChain);
	};

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> p_surfaceFormats)
	{
		for (auto&& l_surfaceFormat : p_surfaceFormats)
		{
			//TODO -> We only check the format here ? Why not adding logici for the colorspace too.
			if (l_surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB)
			{
				return l_surfaceFormat;
			}
		}

		return p_surfaceFormats[0];
	};

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& p_availablePresentModes)
	{
		for (auto&& l_presentMode : p_availablePresentModes)
		{
			if (l_presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return l_presentMode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	};

	VkExtent2D chooseSwapExtent(Window* p_window, const VkSurfaceCapabilitiesKHR& p_capabilities)
	{
		if (p_capabilities.currentExtent.width != UINT32_MAX)
		{
			return p_capabilities.currentExtent;
		}
		else
		{
			WindowSize l_currentWindowSize = Window_getSize(p_window);
			VkExtent2D l_actualExtent{ l_currentWindowSize.Width, l_currentWindowSize.Height };

			l_actualExtent.width = std::max<uint32_t>(p_capabilities.minImageExtent.width, std::min<uint32_t>(p_capabilities.maxImageExtent.width, l_actualExtent.width));
			l_actualExtent.height = std::max<uint32_t>(p_capabilities.minImageExtent.height, std::min<uint32_t>(p_capabilities.maxImageExtent.height, l_actualExtent.height));

			return l_actualExtent;
		}
	};
}
