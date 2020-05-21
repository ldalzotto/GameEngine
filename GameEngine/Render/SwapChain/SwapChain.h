#pragma once

#include <vector>
#include <functional>

#include "Render/Device/Device.h"
#include "Render/Window/Window.h"
#include "Render/Window/Surface.h"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_SwapChain
{

	struct SwapChainDependencies
	{
		_Device::Device* Device;
		_Window::Window* Window;
		_Surface::Surface* Surface;
	};

	struct SwapChain
	{
		VkSwapchainKHR VkSwapchainKHR;
		SwapChainDependencies SwapChainDependencies;

		VkExtent2D SwapExtend;
		VkSurfaceFormatKHR SurfaceFormat;
		VkPresentModeKHR PresentMode;

		std::vector<VkImage> SwapChainImages;
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

	void build(SwapChain* p_swapChain, const SwapChainDependencies& p_swapChainDependencies);
	SwapChainSupportDetails getSwapChainSupportDetails(VkPhysicalDevice p_physicalDevice, _Surface::Surface* p_surface);
	bool isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails);

	void swapChain_free(SwapChain* p_swapChain);

}