#include "SwapChainComposition.h"



namespace _GameEngine::_Render::_SwapChain::_Composition
{
	_SwapChain::GetSwapChainSupportDetailsCallbacks buildSwapChainSupportDetailsCallback(_Surface::Surface* p_surface, VkPhysicalDevice p_physicalDevice)
	{
		_SwapChain::GetSwapChainSupportDetailsCallbacks l_callbacks{};
		l_callbacks.getPhysicalDeviceSurfaceCapabilitiesKHR =
			[p_surface, p_physicalDevice](VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
		{
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_physicalDevice, p_surface->WindowSurface, pSurfaceCapabilities);
		};

		l_callbacks.getPhysicalDeviceSurfaceFormatsKHR =
			[p_surface, p_physicalDevice](uint32_t* p_formatCount, VkSurfaceFormatKHR* p_pSurfaceFormats)
		{
			vkGetPhysicalDeviceSurfaceFormatsKHR(p_physicalDevice, p_surface->WindowSurface, p_formatCount, p_pSurfaceFormats);
		};

		l_callbacks.getPhysicalDeviceSurfacePresentModesKHR =
			[p_surface, p_physicalDevice](uint32_t* p_presentModeCount, VkPresentModeKHR* p_presentModes)
		{
			vkGetPhysicalDeviceSurfacePresentModesKHR(p_physicalDevice, p_surface->WindowSurface, p_presentModeCount, p_presentModes);
		};

		return l_callbacks;
	};

	_SwapChain::GetSwapChainSupportDetailsCallbacks buildSwapChainSupportDetailsCallback(_Surface::Surface* p_surface, _Device::Device* p_device)
	{
		return buildSwapChainSupportDetailsCallback(p_surface, p_device->PhysicalDevice);
	}

	std::function<bool(VkPhysicalDevice p_physicalDevice)> isSwapChainSupported(_Surface::Surface* p_surface)
	{
		return [p_surface](VkPhysicalDevice p_physicalDevice)
		{
			auto l_swapChainSupportDetailsCallback = buildSwapChainSupportDetailsCallback(p_surface, p_physicalDevice);
			_SwapChain::SwapChainSupportDetails l_swapCahinSupportDetails = _SwapChain::getSwapChainSupportDetails(&l_swapChainSupportDetailsCallback);
			return _SwapChain::isSwapChainSupported(&l_swapCahinSupportDetails);
		};
	}

	std::function<_DataStructures::WindowSize()> getCurrentWindowSize(_Window::Window* p_window)
	{
		return [p_window]()
		{
			return _Window::getWindowSize(p_window);
		};
	};

	std::function<void(VkSwapchainCreateInfoKHR*)> feedVkSwapchainCreateInfoKHRWithWindowSurface(_Surface::Surface* p_surface)
	{
		return [p_surface](VkSwapchainCreateInfoKHR* p_swapChainCreateInfo)
		{
			p_swapChainCreateInfo->surface = p_surface->WindowSurface;
		};
	};
}