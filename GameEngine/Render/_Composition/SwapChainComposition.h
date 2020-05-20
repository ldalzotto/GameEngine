#pragma once

#include <functional>

#include "Render/CommonDataStructures/DataStructures.h"

#include "Render/Device/Device.h"
#include "Render/Window/Surface.h"
#include "Render/Window/Window.h"
#include "Render/SwapChain/SwapChain.h"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_SwapChain::_Composition
{
	_SwapChain::GetSwapChainSupportDetailsCallbacks buildSwapChainSupportDetailsCallback(_Surface::Surface* p_surface, _Device::Device* p_device);
	std::function<bool(VkPhysicalDevice p_physicalDevice)> isSwapChainSupported(_Surface::Surface* p_surface);
	std::function<_DataStructures::WindowSize()> getCurrentWindowSize(_Window::Window* p_window);
	std::function<void(VkSwapchainCreateInfoKHR*)> feedVkSwapchainCreateInfoKHRWithWindowSurface(_Surface::Surface* p_window);
}