#pragma once

#include "Window/Window.h"
#include "Window/Surface.h"
#include "vulkan/vulkan.h"
#include "ValidationLayers.h"
#include "Device/Device.h"
#include "SwapChain/SwapChain.h"
#include "GraphcisPipeline/GraphicsPipeline.h"

namespace _GameEngine::_Render
{
	struct RenderDebug
	{
		VkDebugUtilsMessengerEXT DebugMessenger;
		PFN_vkCreateDebugUtilsMessengerEXT PfnCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT PfnDestroyDebugUtilsMessengerEXT;
	};

	struct Render
	{
		_Window::Window Window;
		_Surface::Surface WindowSurface;
		VkInstance Instance;
		_ValidationLayers::ValidationLayers ValidationLayers;
		RenderDebug RenderDebug;
		_Device::Device Device;
		_SwapChain::SwapChain SwapChain;
		_GraphicsPipeline::GraphicsPieline GraphicsPieline;
	};

	Render* alloc();
	void free(Render** p_render);

	void render(Render* p_render);

} // namespace _GameEngine::_Render
