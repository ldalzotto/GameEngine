#pragma once

#include <vector>
#include <functional>

#include "Render/Hardware/Device/Device.h"
#include "Render/Hardware/Window/Window.h"
#include "Render/Hardware/Window/Surface.h"
using namespace _GameEngine::_Render::_Hardware;

#include "Render/CommandBuffer/CommandPool.h"

#include "Render/SwapChain/SwapChainSharedStructures.h"
#include "Render/SwapChain/Image/SwapChainImage.h"

#include "Utils/Observer/Observer.h"
using namespace _GameEngine::_Utils;

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
		SwapChainInfo SwapChainInfo;
		std::vector<_SwapChainImage::SwapChainImage> SwapChainImages;

		/**
			If true, then the @ref SwapChain must be rebuilded.
		*/
		bool IsInvalid;
		_Observer::Subject* OnWindowSizeChangeCallback;
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

	struct SwapChainBuildInfo
	{
		SwapChainDependencies SwapChainDependencies;
		_CommandBuffer::CommandPool* CommandPool;
	};

	void build(SwapChain* p_swapChain, SwapChainBuildInfo* p_swapChainBuildInfo);
	SwapChainSupportDetails getSwapChainSupportDetails(VkPhysicalDevice p_physicalDevice, _Surface::Surface* p_surface);
	bool isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails);
	void swapChain_free(SwapChain* p_swapChain);

}