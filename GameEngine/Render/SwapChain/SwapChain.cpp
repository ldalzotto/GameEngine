#include "SwapChain.h"

#include <algorithm>

namespace _GameEngine::_Render::_SwapChain
{
	SwapChainSupportDetails getSwapChainSupportDetails( VkPhysicalDevice p_physicalDevice, _Surface::Surface* p_surface)
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

	bool isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails)
	{
		return p_swapCahinSupportDetails.PresentModes.size() > 0 && p_swapCahinSupportDetails.SurfaceFormats.size() > 0;
	};

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> p_surfaceFormats)
	{
		for (auto&& l_surfaceFormat : p_surfaceFormats)
		{
			if (l_surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && l_surfaceFormat.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT)
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

	VkExtent2D chooseSwapExtent(_Window::Window* p_window, const VkSurfaceCapabilitiesKHR& p_capabilities)
	{
		if (p_capabilities.currentExtent.width != UINT32_MAX)
		{
			return p_capabilities.currentExtent;
		}
		else
		{
			_Window::WindowSize l_currentWindowSize = _Window::getWindowSize(p_window);
			VkExtent2D l_actualExtent{ l_currentWindowSize.Width, l_currentWindowSize.Height };

			l_actualExtent.width = std::max(p_capabilities.minImageExtent.width, std::min(p_capabilities.maxImageExtent.width, l_actualExtent.width));
			l_actualExtent.height = std::max(p_capabilities.minImageExtent.height, std::min(p_capabilities.maxImageExtent.height, l_actualExtent.height));

			return l_actualExtent;
		}
	};

	void build(SwapChain* p_swapChain, SwapChainCreationStructure* p_swapChainCreationStructure)
	{
		SwapChainSupportDetails l_swapChainDetails = getSwapChainSupportDetails(
			p_swapChainCreationStructure->Device->PhysicalDevice.PhysicalDevice,
			p_swapChainCreationStructure->Surface);

		p_swapChain->SurfaceFormat = chooseSwapSurfaceFormat(l_swapChainDetails.SurfaceFormats);
		p_swapChain->PresentMode = chooseSwapPresentMode(l_swapChainDetails.PresentModes);
		p_swapChain->SwapExtend = chooseSwapExtent(p_swapChainCreationStructure->Window, l_swapChainDetails.Capabilities);

		uint32_t l_imageCount = l_swapChainDetails.Capabilities.minImageCount + 1;
		if (l_swapChainDetails.Capabilities.minImageCount > 0 && l_imageCount > l_swapChainDetails.Capabilities.maxImageCount)
		{
			l_imageCount = l_swapChainDetails.Capabilities.maxImageCount;
		}
		
		VkSwapchainCreateInfoKHR l_createInfo{ };
		l_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		l_createInfo.surface = p_swapChainCreationStructure->Surface->WindowSurface;
		l_createInfo.minImageCount = l_imageCount;
		l_createInfo.imageFormat = p_swapChain->SurfaceFormat.format;
		l_createInfo.imageColorSpace = p_swapChain->SurfaceFormat.colorSpace;
		l_createInfo.imageExtent = p_swapChain->SwapExtend;
		l_createInfo.imageArrayLayers = 1;
		l_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	};

}
