#include "SwapChainComposition.h"

namespace _GameEngine::_Render::_SwapChain::_Composition
{
	std::function<bool(VkPhysicalDevice p_physicalDevice)> isSwapChainSupported(_Surface::Surface* p_surface)
	{
		return [p_surface](VkPhysicalDevice p_physicalDevice)
		{
			_SwapChain::SwapChainSupportDetails l_swapCahinSupportDetails = _SwapChain::getSwapChainSupportDetails(p_physicalDevice, p_surface);
			return _SwapChain::isSwapChainSupported(&l_swapCahinSupportDetails);
		};
	}
}