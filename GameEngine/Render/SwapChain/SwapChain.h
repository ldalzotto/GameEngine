#pragma once

#include <vector>
#include <functional>

#include "Render/CommonDataStructures/DataStructures.h"
#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_SwapChain
{
	struct SwapChain
	{
		VkExtent2D SwapExtend;
		VkSurfaceFormatKHR SurfaceFormat;
		VkPresentModeKHR PresentMode;
	};

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

	struct SwapChainCreationStructure
	{
		std::function<_DataStructures::WindowSize()> GetCurrentWindowSize;
		GetSwapChainSupportDetailsCallbacks* SwapChainSupportDetailsCallbacks;
		std::function<void(VkSwapchainCreateInfoKHR*)> FeedVkSwapchainCreateInfoKHRWithWindowSurface;
	};

	void build(SwapChain* p_swapChain, SwapChainCreationStructure* p_swapChainCreationStructure);
	SwapChainSupportDetails getSwapChainSupportDetails(GetSwapChainSupportDetailsCallbacks* p_callbacks);
	bool isSwapChainSupported(SwapChainSupportDetails* p_swapCahinSupportDetails);

}