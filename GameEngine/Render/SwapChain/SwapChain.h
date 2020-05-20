#pragma once

#include <vector>
#include <functional>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_SwapChain
{
	struct SwapChainSupportDetails
	{
		/**
			Number and dimension of images.
		*/
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> SurfaceFormats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	struct GetSwapChainSupportDetailsCallbacks
	{
		std::function<void(VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)> getPhysicalDeviceSurfaceCapabilitiesKHR;
		std::function<void(uint32_t* p_formatCount, VkSurfaceFormatKHR* p_pSurfaceFormats)> getPhysicalDeviceSurfaceFormatsKHR;
		std::function<void(uint32_t* p_presentModeCount, VkPresentModeKHR* p_presentModes)> getPhysicalDeviceSurfacePresentModesKHR;
	};

	SwapChainSupportDetails getSwapChainSupportDetails(GetSwapChainSupportDetailsCallbacks* p_callbacks);
	bool isSwapChainSupported(SwapChainSupportDetails* p_swapCahinSupportDetails);
}