#pragma once


#include <vector>
#include <functional>

#include "Render/Hardware/Device/Device.h"
#include "Render/Hardware/Window/Window.h"
#include "Render/Hardware/Window/Surface.h"


#include "Render/CommandBuffer/CommandPool.h"

#include "Render/SwapChain/SwapChainSharedStructures.h"
#include "Render/SwapChain/Image/SwapChainImage.h"

#include "Utils/Observer/Observer.h"


#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{

	struct SwapChainDependencies
	{
		Device* Device;
		Window* Window;
		Surface* Surface;
	};

	struct SwapChain
	{
		VkSwapchainKHR VkSwapchainKHR;
		SwapChainDependencies SwapChainDependencies;
		SwapChainInfo SwapChainInfo;
		std::vector<SwapChainImage> SwapChainImages;

		bool MustBeRebuilt;
		_Utils::Subject OnWindowSizeChangeCallback;
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
		CommandPool* CommandPool;
	};

	void SwapChain_build(SwapChain* p_swapChain, SwapChainBuildInfo* p_swapChainBuildInfo);
	SwapChainSupportDetails SwapChain_getSupportDetails(VkPhysicalDevice p_physicalDevice, Surface* p_surface);
	bool SwapChain_isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails);
	void SwapChain_free(SwapChain* p_swapChain);

}