#pragma once

#include <vector>
#include <functional>

#include "VulkanObjects/SwapChain/SwapChainSharedStructures.h"
#include "VulkanObjects/SwapChain/Image/SwapChainImage.h"

#include "Functional/Callback/CallbackT.hpp"
#include "Functional/Callback/ObserverT.hpp"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
	struct Surface;
	struct CommandPool;
}

namespace _GameEngine::_Render
{
	struct SwapChain
	{
		VkSwapchainKHR VkSwapchainKHR;
		RenderInterface* RenderInterface;
		SwapChainInfo SwapChainInfo;
		std::vector<SwapChainImage> SwapChainImages;

		bool MustBeRebuilt;
		_Core::CallbackT<SwapChain, void> OnWindowSizeChangeCallback;

		// The swap chain can be rebuild if the @ref MustBeRebuilt is set to true.
		// This event notify when such an event has occured.
		// Input parameter is (SwapChain*)
		// /!\ WARNING - This observer is not cleaned up when the SwapChain is being rebuilded. It is up to program that register to it to unregister when they need it.
		_Core::ObserverT<_Render::RenderInterface> OnSwapChainBuilded;
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

	void SwapChain_build(SwapChain* p_swapChain, RenderInterface* p_renderInterface);
	SwapChainSupportDetails SwapChain_getSupportDetails(VkPhysicalDevice p_physicalDevice, Surface* p_surface);
	bool SwapChain_isSwapChainSupported(const SwapChainSupportDetails& p_swapCahinSupportDetails);
	void SwapChain_broadcastRebuildEvent(SwapChain* p_swapChain, RenderInterface* p_renderInterface);
	void SwapChain_free(SwapChain* p_swapChain);

}