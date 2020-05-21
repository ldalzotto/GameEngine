#pragma once

#include <vector>
#include <functional>

#include "Render/Device/Device.h"
#include "Render/Window/Window.h"
#include "Render/Window/Surface.h"

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

	struct SwapChainCreationStructure
	{
		_Device::Device* Device;
		_Window::Window* Window;
		_Surface::Surface* Surface;
	};

	void build(SwapChain* p_swapChain, SwapChainCreationStructure* p_swapChainCreationStructure);
	SwapChainSupportDetails getSwapChainSupportDetails(VkPhysicalDevice p_physicalDevice, _Surface::Surface* p_surface);
	bool isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails);

}