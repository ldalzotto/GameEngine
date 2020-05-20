#pragma once

#include "Render/Device/Device.h"
#include "Render/Window/Surface.h"
#include "Render/SwapChain/SwapChain.h"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_SwapChain::_Composition
{
	bool isSwapChainSupported(_Surface::Surface* p_surface, VkPhysicalDevice p_physicalDevice);
}