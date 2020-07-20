#include "Render.h"

#include "VulkanObjects/Extensions/Extensions.h"

#include "Log/Log.hpp"

#include "RenderStep/MaterialDrawStep.h"

#include <stdexcept>

namespace _GameEngine::_Render
{

	bool render_acquireNextRenderInmage(Render* p_render, CurrentSynchronisationObject* p_synchronizationObject, uint32_t* p_outImageIntex);

	inline void render_commandBufferBegin(VkCommandBuffer p_commandBuffer);
	inline void render_commandBufferEnd(VkCommandBuffer p_commandBuffer);

	inline void render_pushToScreen(Render* p_render, CurrentSynchronisationObject* p_synchronizationObject, VkCommandBuffer p_commandBuffer, uint32_t p_imageIndex);

	void Render_render(Render* p_render)
	{
		RenderSemaphore_incrementFrameCount(&p_render->RenderSemaphore);
		CurrentSynchronisationObject l_synchronizationObject = RenderSemaphore_getCurrentSynchronisationObject(&p_render->RenderSemaphore);
		vkResetFences(p_render->Device.LogicalDevice.LogicalDevice, 1, &l_synchronizationObject.WaitForGraphicsQueueFence);

		PreRenderDeferedCommandBufferStep_execute(&p_render->PreRenderDeferedCommandBufferStep, &p_render->Device);

		uint32_t l_imageIndex;
		if (render_acquireNextRenderInmage(p_render, &l_synchronizationObject, &l_imageIndex))
		{

			VkCommandBuffer l_commandBuffer = p_render->SwapChain.SwapChainImages.at(l_imageIndex).CommandBuffer.CommandBuffer;

			render_commandBufferBegin(l_commandBuffer);

			{
				PushCameraBuffer_buildCommandBuffer(&p_render->PushCameraBuffer, l_commandBuffer);
				MaterialDrawStep_buildCommandBuffer(&p_render->RenderInterface, l_commandBuffer, l_imageIndex);

				BeforeEndRecordingMainCommandBuffer_Input l_beforeEndRecordingMainCommandBuffer{};
				l_beforeEndRecordingMainCommandBuffer.CommandBuffer = l_commandBuffer;
				l_beforeEndRecordingMainCommandBuffer.ImageIndex = l_imageIndex;
				l_beforeEndRecordingMainCommandBuffer.RenderInterface = &p_render->RenderInterface;
				_Core::ObserverT_broadcast(&p_render->RenderHookCallbacks.BeforeEndRecordingMainCommandBuffer, &l_beforeEndRecordingMainCommandBuffer);
			}

			render_commandBufferEnd(l_commandBuffer);
			render_pushToScreen(p_render, &l_synchronizationObject, l_commandBuffer, l_imageIndex);
			vkWaitForFences(p_render->Device.LogicalDevice.LogicalDevice, 1, &l_synchronizationObject.WaitForGraphicsQueueFence, VK_FALSE, UINT64_MAX);
		}

		Gizmo_flushDrawStack(&p_render->Gizmo);
	};

	bool render_acquireNextRenderInmage(Render* p_render, CurrentSynchronisationObject* p_synchronizationObject, uint32_t* p_outImageIntex)
	{
		VkResult l_acquireNextImageResult = vkAcquireNextImageKHR(
			p_render->Device.LogicalDevice.LogicalDevice,
			p_render->SwapChain.VkSwapchainKHR,
			99999999,
			p_synchronizationObject->ImageAvailableSemaphore,
			VK_NULL_HANDLE,
			p_outImageIntex);

		if (l_acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR || l_acquireNextImageResult == VK_SUBOPTIMAL_KHR || p_render->SwapChain.MustBeRebuilt)
		{
			Render_recreateSwapChain(p_render);
			return false;
		}

		if (l_acquireNextImageResult != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to acquire swap chain image!"));
		}

		return true;
	};

	inline void render_commandBufferBegin(VkCommandBuffer p_commandBuffer)
	{
		VkCommandBufferBeginInfo l_commandBufferBeginInfo{};
		l_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		l_commandBufferBeginInfo.flags = 0;
		l_commandBufferBeginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(p_commandBuffer, &l_commandBufferBeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to begin recording command buffer!"));
		}
	};
	
	inline void render_commandBufferEnd(VkCommandBuffer p_commandBuffer)
	{
		if (vkEndCommandBuffer(p_commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to record command buffer!"));
		}
	};

	inline void render_pushToScreen(Render* p_render, CurrentSynchronisationObject* p_synchronizationObject, VkCommandBuffer p_commandBuffer, uint32_t p_imageIndex)
	{
		VkSubmitInfo l_submitInfo{};
		l_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore l_waitSemaphore[] = { p_synchronizationObject->ImageAvailableSemaphore };
		VkPipelineStageFlags l_waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		l_submitInfo.waitSemaphoreCount = 1;
		l_submitInfo.pWaitSemaphores = l_waitSemaphore;
		l_submitInfo.pWaitDstStageMask = l_waitStages;
		l_submitInfo.commandBufferCount = 1;
		l_submitInfo.pCommandBuffers = &p_commandBuffer;

		VkSemaphore l_signalSemaphores[] = { p_synchronizationObject->RenderFinishedSemaphore };
		l_submitInfo.signalSemaphoreCount = 1;
		l_submitInfo.pSignalSemaphores = l_signalSemaphores;

		if (vkQueueSubmit(p_render->Device.LogicalDevice.Queues.GraphicsQueue, 1, &l_submitInfo, p_synchronizationObject->WaitForGraphicsQueueFence) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to submit draw command buffer!"));
		}

		VkPresentInfoKHR l_presentInfo{ };
		l_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		l_presentInfo.waitSemaphoreCount = 1;
		l_presentInfo.pWaitSemaphores = l_signalSemaphores;

		VkSwapchainKHR l_swapChains[] = { p_render->SwapChain.VkSwapchainKHR };
		l_presentInfo.swapchainCount = 1;
		l_presentInfo.pSwapchains = l_swapChains;
		l_presentInfo.pImageIndices = &p_imageIndex;

		l_presentInfo.pResults = nullptr;

		vkQueuePresentKHR(p_render->Device.LogicalDevice.Queues.PresentQueue, &l_presentInfo);
	};

	void initializeVulkan(Render* p_render);
	void freeVulkan(Render* p_render);

	void initValidationLayers(Render* p_render);

	void initVulkanDebugger(Render* p_render);
	void initVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT* p_debugUtilsMessengerCreateInfo, RenderInterface* p_renderInterface);
	void freeVulkanDebugger(Render* p_render);

	void initSurface(Render* p_render);
	void freeSurface(Render* p_render);

	void initDevice(Render* p_render);
	void freeDevice(Render* p_render);

	void initSwapChain(Render* p_render);
	void freeSwapChain(Render* p_render);

	void initDepthTexture(Render* p_render);
	void freeDepthTexture(Render* p_render);

	void reAllocateGraphicsPipelineContainer(Render* p_render);

	void initCommandPool(Render* p_render);
	void freeCommandPool(Render* p_render);

	void initRenderSemaphore(Render* p_render);
	void freeRenderSemaphore(Render* p_render);

	void initTextureSamplers(Render* p_render);
	void freeTextureSamplers(Render* p_render);

	void initResourcesProvider(Render* p_render);
	void freeResourcesProvider(Render* p_render);

	void initPreRenderStaging(Render* p_render);
	void freePreRenderStaging(Render* p_render);


	void Render_build(Render* p_render, ::_Core::Log* p_myLog)
	{
		RenderInterface_initialize(p_render, p_myLog);
		RenderHookCallbacks_alloc(&p_render->RenderHookCallbacks);

		Window_init(&p_render->Window);

		initValidationLayers(p_render);
		initializeVulkan(p_render);
		initVulkanDebugger(p_render);
		initSurface(p_render);
		initDevice(p_render);
		initCommandPool(p_render);
		initSwapChain(p_render);
		initRenderSemaphore(p_render);
		initTextureSamplers(p_render);
		initPreRenderStaging(p_render);
		initDepthTexture(p_render);
		initResourcesProvider(p_render);
		PushCameraBuffer_init(&p_render->PushCameraBuffer, &p_render->Device);
		MaterialInstanceContainer_alloc(&p_render->MaterialInstanceContainer);
		Gizmo_alloc(&p_render->Gizmo, &p_render->RenderInterface);
		p_render->MaterialInstanceContainer.RenderInterface = &p_render->RenderInterface;

		SwapChain_broadcastRebuildEvent(&p_render->SwapChain, &p_render->RenderInterface);
	};

	void Render_free(Render* p_render)
	{
		// We wait for the Queues to finish their curent operation before releasing memory.
		// This is to ensure that no undefined behavior occurs while doing so.
		vkDeviceWaitIdle((p_render)->Device.LogicalDevice.LogicalDevice);

		Gizmo_free(&(p_render)->Gizmo, &(p_render)->RenderInterface);
		MaterialInstanceContainer_free(&(p_render)->MaterialInstanceContainer);
		PushCameraBuffer_free(&(p_render)->PushCameraBuffer, &(p_render)->Device);
		freeResourcesProvider(p_render);
		freeDepthTexture(p_render);
		freePreRenderStaging(p_render);
		freeTextureSamplers(p_render);
		freeRenderSemaphore(p_render);
		freeSwapChain(p_render);
		TextureSwapChainSizeSynchronizer_free(&(p_render)->TextureSwapChainSizeSynchronizer, &p_render->RenderInterface);
		freeCommandPool(p_render);
		freeDevice(p_render);
		freeSurface(p_render);
		freeVulkanDebugger((p_render));
		freeVulkan(p_render);

		RenderHookCallbacks_free(&p_render->RenderHookCallbacks);

		Window_closeWindow(&p_render->Window);
	};

	void Render_recreateSwapChain(Render* p_render)
	{
		// We wait for the Queues to finish their curent operation before releasing memory.
		// This is to ensure that no undefined behavior occurs while doing so.
		vkDeviceWaitIdle(p_render->Device.LogicalDevice.LogicalDevice);

		freeRenderSemaphore(p_render);
		freeSwapChain(p_render);
		freePreRenderStaging(p_render);
		freeCommandPool(p_render);

		initCommandPool(p_render);
		initPreRenderStaging(p_render);
		initSwapChain(p_render);
		TextureSwapChainSizeSynchronizer_onSwapChainSizeChanged(&p_render->TextureSwapChainSizeSynchronizer);
		reAllocateGraphicsPipelineContainer(p_render);
		initRenderSemaphore(p_render);

		// The SwapChain has been recreated, thus no more invalid
		SwapChain_broadcastRebuildEvent(&p_render->SwapChain, &p_render->RenderInterface);
	};

	/////// VULKAN

	void initializeVulkan(Render* p_render)
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "GameEngine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		std::vector<char*> l_requiredExtensions = Window_getRequiredExtensionsV2(&p_render->Window);
		Extensions_checkPresenceOfRequiredInstanceExtensions(l_requiredExtensions);
		Extensions_populateRequiredExtensions(&l_requiredExtensions, p_render->ValidationLayers.EnableValidationLayers);

		createInfo.enabledExtensionCount = static_cast<uint32_t>(l_requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = l_requiredExtensions.data();

		// /!\ This messenger create info is used to log any problems in the creation and destroy of vulkan instance.
		//	   It is placed outside the if because vulakn only accepts a pointer to it, so this is to avoid stack destruction.
		VkDebugUtilsMessengerCreateInfoEXT l_debugUtlsMessengerCreateInfo{};

		ValidationLayers* l_validationLayers = &p_render->ValidationLayers;
		if (l_validationLayers->EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(l_validationLayers->ValidationLayers.size());
			createInfo.ppEnabledLayerNames = l_validationLayers->ValidationLayers.data();

			initVkDebugUtilsMessengerCreateInfoEXT(&l_debugUtlsMessengerCreateInfo, &p_render->RenderInterface);
			createInfo.pNext = &l_debugUtlsMessengerCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &p_render->Instance) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance.");
		}
	}

	void freeVulkan(Render* p_render)
	{
		vkDestroyInstance(p_render->Instance, nullptr);
	};

	////// END VULKAN

	/////// VALIDATION LAYERS

	void initValidationLayers(Render* p_render)
	{
		ValidationLayer_init(&p_render->ValidationLayers);
		ValidationLayer_checkSupport(&p_render->ValidationLayers);
	};

	/////// END VALIDATION LAYERS

	/////// VULKAN DEBUGGER

	void initVulkanDebugger(Render* p_render)
	{
		if (p_render->ValidationLayers.EnableValidationLayers)
		{
			RenderDebug* l_renderDebug = &p_render->RenderDebug;
			l_renderDebug->PfnCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(p_render->Instance, "vkCreateDebugUtilsMessengerEXT");
			l_renderDebug->PfnDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(p_render->Instance, "vkDestroyDebugUtilsMessengerEXT");


			VkDebugUtilsMessengerCreateInfoEXT createInfo{};
			initVkDebugUtilsMessengerCreateInfoEXT(&createInfo, &p_render->RenderInterface);

			if (l_renderDebug->PfnCreateDebugUtilsMessengerEXT(p_render->Instance, &createInfo, nullptr, &l_renderDebug->DebugMessenger) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to set up debug message!");
			}
		}
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT p_messageSeverity, VkDebugUtilsMessageTypeFlagsEXT p_messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* p_callbackData, void* p_userData)
	{
		RenderInterface* l_renderInterface = (RenderInterface*)p_userData;

		switch (p_messageSeverity)
		{
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			MYLOG_PUSH(l_renderInterface->MyLog, ::_Core::LogLevel::INFO, (char*)p_callbackData->pMessage);
			break;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			MYLOG_PUSH(l_renderInterface->MyLog, ::_Core::LogLevel::INFO, (char*)p_callbackData->pMessage);
			break;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			MYLOG_PUSH(l_renderInterface->MyLog, ::_Core::LogLevel::WARN, (char*)p_callbackData->pMessage);
			break;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			MYLOG_PUSH(l_renderInterface->MyLog, ::_Core::LogLevel::ERROR, (char*)p_callbackData->pMessage);
			break;
		}

		return VK_FALSE;
	};

	void initVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT* p_debugUtilsMessengerCreateInfo, RenderInterface* p_renderInterface)
	{
		p_debugUtilsMessengerCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		p_debugUtilsMessengerCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		p_debugUtilsMessengerCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		p_debugUtilsMessengerCreateInfo->pfnUserCallback = debugCallback;
		p_debugUtilsMessengerCreateInfo->pUserData = p_renderInterface;
	};

	void freeVulkanDebugger(Render* p_render)
	{
		if (p_render->ValidationLayers.EnableValidationLayers)
		{
			p_render->RenderDebug.PfnDestroyDebugUtilsMessengerEXT(p_render->Instance, p_render->RenderDebug.DebugMessenger, nullptr);
		}
	};

	/////// END VULKAN DEBUGGER

	/////// SURFACE

	void initSurface(Render* p_render)
	{
		Surface_build(&p_render->WindowSurface, &p_render->RenderInterface);
	};

	void freeSurface(Render* p_render)
	{
		Surface_release(&p_render->WindowSurface, &p_render->RenderInterface);
	};

	/////// END SURFACE

	/////// DEVICE

	void setupLogicalDeviceValidation(ValidationLayers* p_validationLayers, VkDeviceCreateInfo* p_deviceCreateInfo);

	void initDevice(Render* p_render)
	{
		DeviceBuildInfo l_deviceBuildInfo{};
		l_deviceBuildInfo.Instance = p_render->Instance;

		DeviceBuildCallbacks* l_deviceBuildCallbacks = &l_deviceBuildInfo.DeviceBuildCallbacks;

		l_deviceBuildCallbacks->SetupValidation = [p_render](VkDeviceCreateInfo* p_deviceCreateInfo) {
			setupLogicalDeviceValidation(&p_render->ValidationLayers, p_deviceCreateInfo);
		};
		l_deviceBuildCallbacks->GetPhysicalDeviceSurfaceSupport = [p_render](VkPhysicalDevice p_device, uint32_t p_queueFamilyIndex, VkBool32* p_supported)
		{
			return vkGetPhysicalDeviceSurfaceSupportKHR(p_device, p_queueFamilyIndex, p_render->WindowSurface.WindowSurface, p_supported);
		};
		l_deviceBuildCallbacks->IsSwapChainSupported = [p_render](VkPhysicalDevice p_physicalDevice)
		{
			return SwapChain_isSwapChainSupported(SwapChain_getSupportDetails(p_physicalDevice, &p_render->WindowSurface));
		};

		Device_build(&p_render->Device, &l_deviceBuildInfo);
	};

	void setupLogicalDeviceValidation(ValidationLayers* p_validationLayers, VkDeviceCreateInfo* p_deviceCreateInfo)
	{
		if (p_validationLayers->EnableValidationLayers)
		{
			p_deviceCreateInfo->enabledLayerCount = static_cast<uint32_t>(p_validationLayers->ValidationLayers.size());
			p_deviceCreateInfo->ppEnabledLayerNames = p_validationLayers->ValidationLayers.data();
		}
		else
		{
			p_deviceCreateInfo->enabledLayerCount = 0;
		}
	};

	void freeDevice(Render* p_render)
	{
		Device_free(&p_render->Device);
	};

	/////// END DEVICE

	/////// SWAP CHAIN

	void initSwapChain(Render* p_render)
	{
		SwapChain_build(&p_render->SwapChain, &p_render->RenderInterface);
		SwapChain_broadcastRebuildEvent(&p_render->SwapChain, &p_render->RenderInterface);
	};

	void freeSwapChain(Render* p_render)
	{
		SwapChain_free(&p_render->SwapChain);
	};

	/////// END SWAP CHAIN

	/////// DEPTH TEXTURE

	void initDepthTexture(Render* p_render)
	{
		TextureAllocInfo l_textureAllocInfo{};
		l_textureAllocInfo.TextureAllocationType = TextureAllocationType::PROCEDURAL;
		l_textureAllocInfo.TextureKey.TexturePath = "DEPTH";
		l_textureAllocInfo.TextureCreateInfo.TextureType = TextureType::DEPTH;
		l_textureAllocInfo.TextureCreateInfo.TextureUsage = TextureUsage::PIPELINE_ATTACHMENT;
		l_textureAllocInfo.FitSwapChainSize = true;
		l_textureAllocInfo.RenderInterface = &p_render->RenderInterface;
		p_render->DepthTexture = Texture_alloc(&l_textureAllocInfo);
	};

	void freeDepthTexture(Render* p_render)
	{
		Texture_free(&p_render->DepthTexture, &p_render->RenderInterface);
	};

	/////// END DEPTH TEXTURE

	/////// GRAPHICS PIPELINE

	void reAllocateGraphicsPipelineContainer(Render* p_render)
	{
		MaterialInstanceContainer_reAllocGraphicsPipeline(&p_render->MaterialInstanceContainer);
	};

	/////// END GRAPHICS PIPELINE

	/////// COMMAND POOL

	void initCommandPool(Render* p_render)
	{
		CommandPoolDependencies l_commandPoolDependencies{};
		l_commandPoolDependencies.Device = &p_render->Device;
		CommandPool_Init(&p_render->CommandPool, &l_commandPoolDependencies);
	};

	void freeCommandPool(Render* p_render)
	{
		CommandPool_free(&p_render->CommandPool);
	};

	/////// END COMMAND POOL

	/////// RENDER SEMAPHORE

	void initRenderSemaphore(Render* p_render)
	{
		RenderSemaphoreDependencies l_renderSemaphoreDependencies{ };
		l_renderSemaphoreDependencies.Device = &p_render->Device;

		RenderSemaphoreCreationInfo l_renderSemaphoreCreationInfo{};
		l_renderSemaphoreCreationInfo.MaxFramesInParallel = 2;
		l_renderSemaphoreCreationInfo.RenderSemaphoreDependencies = &l_renderSemaphoreDependencies;

		RenderSemaphore_init(&p_render->RenderSemaphore, &l_renderSemaphoreCreationInfo);
	};

	void freeRenderSemaphore(Render* p_render)
	{
		RenderSemaphore_free(&p_render->RenderSemaphore);
	};

	/////// END RENDER SEMAPHORE

	/////// TEXTURE SAMPLERS

	void initTextureSamplers(Render* p_render)
	{
		TextureSamplers_alloc(&p_render->TextureSamplers, &p_render->Device);
	};

	void freeTextureSamplers(Render* p_render)
	{
		TextureSamplers_free(&p_render->TextureSamplers, &p_render->Device);
	};

	/////// END TEXTURE SAMPLERS

	/////// RESOURCES PROVIDER

	void initResourcesProvider(Render* p_render)
	{
		p_render->ResourceProviders.TextureResourceProvider.RenderInterface = &p_render->RenderInterface;
		p_render->ResourceProviders.MeshResourceProvider.RenderInterface = &p_render->RenderInterface;
		p_render->ResourceProviders.MaterialResourceProvider.RenderInterface = &p_render->RenderInterface;
	};

	void freeResourcesProvider(Render* p_render)
	{
		TextureResourceProvider_Clear(&p_render->ResourceProviders.TextureResourceProvider);
		MeshResourceProvider_Clear(&p_render->ResourceProviders.MeshResourceProvider);
		MaterialResourceProvider_Clear(&p_render->ResourceProviders.MaterialResourceProvider);
	};

	/////// END RESOURCES PROVIDER

	/////// PRE RENDER STAGGING

	void initPreRenderStaging(Render* p_render)
	{
		PreRenderDeferedCommandBufferStep_alloc(&p_render->PreRenderDeferedCommandBufferStep, &p_render->CommandPool);
	};

	void freePreRenderStaging(Render* p_render)
	{
		PreRenderDeferedCommandBufferStep_free(&p_render->PreRenderDeferedCommandBufferStep, &p_render->Device);
	};

	/////// END PRE RENDER STAGGING

} // namespace _GameEngine