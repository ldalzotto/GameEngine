#include "SwapChain.h"

namespace _GameEngine::_Render::_SwapChain
{
	SwapChainSupportDetails getSwapChainSupportDetails(GetSwapChainSupportDetailsCallbacks* p_callbacks)
	{
		SwapChainSupportDetails l_swapChainSupportDetails{};

		p_callbacks->getPhysicalDeviceSurfaceCapabilitiesKHR(&l_swapChainSupportDetails.Capabilities);

		uint32_t l_deviceSurfaceFormatCount;
		p_callbacks->getPhysicalDeviceSurfaceFormatsKHR(&l_deviceSurfaceFormatCount, nullptr);

		if (l_deviceSurfaceFormatCount > 0)
		{
			l_swapChainSupportDetails.SurfaceFormats = std::vector<VkSurfaceFormatKHR>(l_deviceSurfaceFormatCount);
			p_callbacks->getPhysicalDeviceSurfaceFormatsKHR(&l_deviceSurfaceFormatCount, l_swapChainSupportDetails.SurfaceFormats.data());
		}

		uint32_t l_presentModeCount;
		p_callbacks->getPhysicalDeviceSurfacePresentModesKHR(&l_presentModeCount, nullptr);
		if (l_presentModeCount > 0)
		{
			l_swapChainSupportDetails.PresentModes = std::vector<VkPresentModeKHR>(l_presentModeCount);
			p_callbacks->getPhysicalDeviceSurfacePresentModesKHR(&l_presentModeCount, l_swapChainSupportDetails.PresentModes.data());
		}

		return l_swapChainSupportDetails;
	};

	bool isSwapChainSupported(SwapChainSupportDetails* p_swapCahinSupportDetails)
	{
		return p_swapCahinSupportDetails->PresentModes.size() > 0 && p_swapCahinSupportDetails->SurfaceFormats.size() > 0;
	};
}
