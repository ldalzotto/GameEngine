#pragma once


#include "VulkanObjects/Hardware/Window/Window.h"
#include "VulkanObjects/Hardware/Window/Surface.h"

#include "vulkan/vulkan.h"

#include "RenderHook.h"
#include "Texture/Texture.h"
#include "VulkanObjects/ValidationLayers.h"
#include "VulkanObjects/Hardware/Device/Device.h"
#include "VulkanObjects/SwapChain/SwapChain.h"
#include "VulkanObjects/Descriptor/DescriptorPool.h"
#include "VulkanObjects/CommandBuffer/CommandPool.h"
#include "VulkanObjects/CommandBuffer/CommandBuffers.h"
#include "VulkanObjects/Synchronisation/Semaphore.h"
#include "Texture/TextureSamplers.h"
#include "Texture/TextureSwapChainSizeSynchronizer.h"
#include "LoopStep/PreRenderDeferedCommandBufferStep.h"
#include "LoopStep/CameraBufferSetupStep.h"
#include "Resources/TextureResourceProvider.h"
#include "Resources/MeshResourceProvider.h"
#include "Resources/MaterialResourceProvider.h"
#include "Materials/MaterialInstanceContainer.h"
#include "Gizmo/Gizmo.h"

#include "RenderInterface.h"

namespace _GameEngine::_Render
{
	struct RenderDebug
	{
		VkDebugUtilsMessengerEXT DebugMessenger;
		PFN_vkCreateDebugUtilsMessengerEXT PfnCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT PfnDestroyDebugUtilsMessengerEXT;
	};

	struct ResourceProviders
	{
		TextureResourceProvider TextureResourceProvider;
		MeshResourceProvider MeshResourceProvider;
		MaterialResourceProvider MaterialResourceProvider;
	};

	struct Render
	{
		RenderInterface RenderInterface;
		RenderHookCallbacks RenderHookCallbacks;

		Window Window;
		Surface WindowSurface;
		VkInstance Instance;
		ValidationLayers ValidationLayers;
		RenderDebug RenderDebug;
		Device Device;
		SwapChain SwapChain;
		Texture* DepthTexture;
		CommandPool CommandPool;
		RenderSemaphore RenderSemaphore;
		TextureSamplers TextureSamplers;
		TextureSwapChainSizeSynchronizer TextureSwapChainSizeSynchronizer;
		MaterialInstanceContainer MaterialInstanceContainer;
		ResourceProviders ResourceProviders;

		PreRenderDeferedCommandBufferStep PreRenderDeferedCommandBufferStep;
		CameraBufferSetupStep CameraBufferSetupStep;
		Gizmo Gizmo;
	};

	Render* Render_alloc();
	void Render_free(Render** p_render);
	void Render_recreateSwapChain(Render* p_render);
	
	void Render_render(Render* p_render);

} // namespace _GameEngine::_Render
