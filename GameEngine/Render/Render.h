#pragma once

#include "Render/Hardware/Window/Window.h"
#include "Render/Hardware/Window/Surface.h"
using namespace _GameEngine::_Render::_Hardware;

#include "vulkan/vulkan.h"
#include "ValidationLayers.h"
#include "Hardware/Device/Device.h"
#include "SwapChain/SwapChain.h"
#include "GraphcisPipeline/GraphicsPipeline.h"
#include "CommandBuffer/CommandPool.h"
#include "CommandBuffer/CommandBuffers.h"
#include "Synchronisation/Semaphore.h"

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
		_GraphicsPipeline::GraphicsPipeline GraphicsPipeline;
		_CommandBuffer::CommandPool CommandPool;
		_CommandBuffer::CommandBuffer CommandBuffers;
		_Synchronisation::RenderSemaphore RenderSemaphore;
	};

	Render* alloc();
	void free(Render** p_render);
	void recreateSwapChain(Render* p_render);

	void render(Render* p_render);

} // namespace _GameEngine::_Render
