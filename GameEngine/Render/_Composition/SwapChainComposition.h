#pragma once

#include <functional>

#include "Render/Window/Surface.h"
#include "Render/SwapChain/SwapChain.h"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_SwapChain::_Composition
{
	std::function<bool(VkPhysicalDevice p_physicalDevice)> isSwapChainSupported(_Surface::Surface* p_surface);
}