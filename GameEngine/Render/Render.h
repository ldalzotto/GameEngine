#pragma once


#include "Render/Hardware/Window/Window.h"
#include "Render/Hardware/Window/Surface.h"


#include "vulkan/vulkan.h"
#include "ValidationLayers.h"
#include "Hardware/Device/Device.h"
#include "SwapChain/SwapChain.h"
#include "GraphcisPipeline/GraphicsPipeline.h"
#include "Descriptor/DescriptorPool.h"
#include "CommandBuffer/CommandPool.h"
#include "CommandBuffer/CommandBuffers.h"
#include "Synchronisation/Semaphore.h"
#include "LoopStep/PreRenderStagingStep.h"
#include "LoopStep/MeshDrawStep.h"

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
		Window Window;
		Surface WindowSurface;
		VkInstance Instance;
		ValidationLayers ValidationLayers;
		RenderDebug RenderDebug;
		Device Device;
		SwapChain SwapChain;
		GraphicsPipeline GraphicsPipeline;
		CommandPool CommandPool;
		CommandBuffer CommandBuffers;
		RenderSemaphore RenderSemaphore;
		DescriptorPool DescriptorPool;

		PreRenderStagingStep PreRenderStagging;
		MeshDrawStep MeshDrawStep;
	};

	Render* Render_alloc();
	void Render_free(Render** p_render);
	void Render_recreateSwapChain(Render* p_render);

	void Render_render(Render* p_render);

} // namespace _GameEngine::_Render
