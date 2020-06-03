#include "Render.h"

#include "VulkanObjects/Extensions/Extensions.h"
#include "Log/Log.h"

#include "Render/IMGUITest/IMGUITest.h"
#include "Render/Texture/InitializationConfigurations/TCDepth.h"

#include <stdexcept>

namespace _GameEngine::_Render
{
	void initializeVulkan(Render* p_render);
	void freeVulkan(Render* p_render);

	void initValidationLayers(Render* p_render);

	void initVulkanDebugger(Render* p_render);
	void initVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT* p_debugUtilsMessengerCreateInfo);
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

	void allocMaterials(Render* p_render);
	void freeMaterials(Render* p_render);

	void allocDefaultMaterialRenderStep(Render* p_render);
	void freeDefaultMaterialRenderStep(Render* p_render);


	Render* Render_alloc()
	{
		Render* l_render = new Render();
		RenderInterface_initialize(l_render);
		
		Window_init(&l_render->Window);

		initValidationLayers(l_render);
		initializeVulkan(l_render);
		initVulkanDebugger(l_render);
		initSurface(l_render);
		initDevice(l_render);
		initCommandPool(l_render);
		initSwapChain(l_render);
		initRenderSemaphore(l_render);
		initTextureSamplers(l_render);
		initPreRenderStaging(l_render);
		initDepthTexture(l_render);
		initResourcesProvider(l_render);
		CameraBufferSetupStep_init(&l_render->CameraBufferSetupStep, &l_render->Device);
		allocMaterials(l_render);
		allocDefaultMaterialRenderStep(l_render);

		IMGUITest_init(&l_render->IMGUITest, &l_render->RenderInterface);

		return l_render;
	};

	void Render_free(Render** p_render)
	{
		// We wait for the Queues to finish their curent operation before releasing memory.
		// This is to ensure that no undefined behavior occurs while doing so.
		vkDeviceWaitIdle((*p_render)->Device.LogicalDevice.LogicalDevice);

		IMGUITest_free(&(*p_render)->IMGUITest, &(*p_render)->RenderInterface);
		freeDefaultMaterialRenderStep(*p_render);
		freeMaterials(*p_render);
		CameraBufferSetupStep_free(&(*p_render)->CameraBufferSetupStep, &(*p_render)->Device);
		freeResourcesProvider(*p_render);
		freeDepthTexture(*p_render);
		freePreRenderStaging(*p_render);
		freeTextureSamplers(*p_render);
		freeRenderSemaphore(*p_render);
		freeSwapChain(*p_render);
		freeCommandPool(*p_render);
		freeDevice(*p_render);
		freeSurface(*p_render);
		freeVulkanDebugger((*p_render));
		freeVulkan(*p_render);
		delete* p_render;
		*p_render = nullptr;
	};

	void Render_recreateSwapChain(Render* p_render)
	{
		// We wait for the Queues to finish their curent operation before releasing memory.
		// This is to ensure that no undefined behavior occurs while doing so.
		vkDeviceWaitIdle(p_render->Device.LogicalDevice.LogicalDevice);

		freeRenderSemaphore(p_render);
		freeSwapChain(p_render);
		freeDepthTexture(p_render);
		freePreRenderStaging(p_render);
		freeCommandPool(p_render);

		initCommandPool(p_render);
		initPreRenderStaging(p_render);
		initSwapChain(p_render);
		initDepthTexture(p_render);
		IMGUITest_onSwapChainRebuilded(&p_render->IMGUITest, &p_render->RenderInterface);
		reAllocateGraphicsPipelineContainer(p_render);
		initRenderSemaphore(p_render);

		// The SwapChain has been recreated, thus no more invalid
		p_render->SwapChain.MustBeRebuilt = false;
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

			initVkDebugUtilsMessengerCreateInfoEXT(&l_debugUtlsMessengerCreateInfo);
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
			initVkDebugUtilsMessengerCreateInfoEXT(&createInfo);

			if (l_renderDebug->PfnCreateDebugUtilsMessengerEXT(p_render->Instance, &createInfo, nullptr, &l_renderDebug->DebugMessenger) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to set up debug message!");
			}
		}
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT p_messageSeverity, VkDebugUtilsMessageTypeFlagsEXT p_messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* p_callbackData, void* p_userData)
	{
		switch (p_messageSeverity)
		{
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			_Log::LogInstance->CoreLogger->trace(p_callbackData->pMessage);
			break;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			_Log::LogInstance->CoreLogger->info(p_callbackData->pMessage);
			break;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			_Log::LogInstance->CoreLogger->warn(p_callbackData->pMessage);
			break;
		case VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			_Log::LogInstance->CoreLogger->error(p_callbackData->pMessage);
			break;
		}

		return VK_FALSE;
	};

	void initVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT* p_debugUtilsMessengerCreateInfo)
	{
		p_debugUtilsMessengerCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		p_debugUtilsMessengerCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		p_debugUtilsMessengerCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		p_debugUtilsMessengerCreateInfo->pfnUserCallback = debugCallback;
		p_debugUtilsMessengerCreateInfo->pUserData = nullptr;
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
		SwapChainBuildInfo l_swapChainBuildInfo{};
		l_swapChainBuildInfo.SwapChainDependencies.Device = &p_render->Device;
		l_swapChainBuildInfo.SwapChainDependencies.Surface = &p_render->WindowSurface;
		l_swapChainBuildInfo.SwapChainDependencies.Window = &p_render->Window;
		l_swapChainBuildInfo.CommandPool = &p_render->CommandPool;
		SwapChain_build(&p_render->SwapChain, &l_swapChainBuildInfo);
	};

	void freeSwapChain(Render* p_render)
	{
		SwapChain_free(&p_render->SwapChain);
	};

	/////// END SWAP CHAIN

	/////// DEPTH TEXTURE

	void initDepthTexture(Render* p_render)
	{
		TextureUniqueKey l_textureKey{ "DEPTH" };

		TextureProceduralInstanceInfo l_textureProceduralInstanceInfo{};
		l_textureProceduralInstanceInfo.TextureKey = &l_textureKey;
		l_textureProceduralInstanceInfo.Width = p_render->SwapChain.SwapChainInfo.SwapExtend.width;
		l_textureProceduralInstanceInfo.Height = p_render->SwapChain.SwapChainInfo.SwapExtend.height;
		l_textureProceduralInstanceInfo.ImageCreateInfoProvider = TCDepth_BuildVkImageCreateInfo;
		l_textureProceduralInstanceInfo.ImageViewCreationInfoProvider = TCDepth_BuildVkImageViewCreateInfo;
		l_textureProceduralInstanceInfo.AllocDeferredCommandBufferOperation = TCDepth_InitializationCommandBufferOperation_build;

		l_textureProceduralInstanceInfo.Device = &p_render->Device;
		l_textureProceduralInstanceInfo.PreRenderDeferedCommandBufferStep = &p_render->PreRenderDeferedCommandBufferStep;

		p_render->DepthTexture = Texture_proceduralInstance(&l_textureProceduralInstanceInfo);
	};

	void freeDepthTexture(Render* p_render)
	{
		Texture_free(&p_render->DepthTexture, &p_render->Device, &p_render->PreRenderDeferedCommandBufferStep);
	};

	/////// END DEPTH TEXTURE

	/////// GRAPHICS PIPELINE

	void reAllocateGraphicsPipelineContainer(Render* p_render)
	{
		DefaultMaterial_reAllocGraphicsPipeline(&p_render->RenderMaterials.DefaultMaterial, &p_render->RenderInterface);
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
		p_render->ResourceProviders.TextureResourceProvider.TextureResourceProviderDependencies.Device = &p_render->Device;
		p_render->ResourceProviders.TextureResourceProvider.TextureResourceProviderDependencies.PreRenderDeferedCommandBufferStep = &p_render->PreRenderDeferedCommandBufferStep;

		p_render->ResourceProviders.MeshResourceProvider.MeshResourceProviderDependencies.Device = &p_render->Device;
		p_render->ResourceProviders.MeshResourceProvider.MeshResourceProviderDependencies.PreRenderDeferedCommandBufferStep = &p_render->PreRenderDeferedCommandBufferStep;
	};

	void freeResourcesProvider(Render* p_render)
	{
		TextureResourceProvider_Clear(&p_render->ResourceProviders.TextureResourceProvider);
		MeshResourceProvider_Clear(&p_render->ResourceProviders.MeshResourceProvider);
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

	/////// MATERIALS

	void allocMaterials(Render* p_render)
	{
		DefaultMaterial_alloc(&p_render->RenderMaterials.DefaultMaterial, &p_render->RenderInterface);
	};

	void freeMaterials(Render* p_render)
	{
		DefaultMaterial_free(&p_render->RenderMaterials.DefaultMaterial, &p_render->RenderInterface);
	};

	/////// END MATERIALS

	/////// MESH RENDER STEP

	void allocDefaultMaterialRenderStep(Render* p_render)
	{
		DefaultMaterialDrawStep_init(&p_render->DefaultMaterialDrawStep, &p_render->RenderMaterials.DefaultMaterial);
	};

	void freeDefaultMaterialRenderStep(Render* p_render)
	{
		DefaultMaterialDrawStep_clear(&p_render->DefaultMaterialDrawStep);
	};

	/////// END MESH RENDER STEP

	void preRenderStagginStep(Render* p_render)
	{

		if (PreRenderDeferedCommandBufferStep_buildCommandBuffer(&p_render->PreRenderDeferedCommandBufferStep, &p_render->Device) & PreRenderDeferredCommandBufferStepStatusBitFlag::CREATED)
		{
			VkSubmitInfo l_staginSubmit{};
			l_staginSubmit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			l_staginSubmit.commandBufferCount = 1;
			l_staginSubmit.pCommandBuffers = &p_render->PreRenderDeferedCommandBufferStep.DedicatedCommandBuffer.CommandBuffer;
			vkQueueSubmit(p_render->Device.LogicalDevice.Queues.GraphicsQueue, 1, &l_staginSubmit, p_render->PreRenderDeferedCommandBufferStep.PreRenderStaggingFence);

			PreRenderDeferedCommandBufferStep_WaitForFence(&p_render->PreRenderDeferedCommandBufferStep, &p_render->Device);
		}
	};

	void Render_render(Render* p_render)
	{
		RenderSemaphore_incrementFrameCount(&p_render->RenderSemaphore);
		CurrentSynchronisationObject l_synchronizationObject = RenderSemaphore_getCurrentSynchronisationObject(&p_render->RenderSemaphore);
		vkResetFences(p_render->Device.LogicalDevice.LogicalDevice, 1, &l_synchronizationObject.WaitForGraphicsQueueFence);

		preRenderStagginStep(p_render);

		uint32_t l_imageIndex;
		VkResult l_acquireNextImageResult = vkAcquireNextImageKHR(
			p_render->Device.LogicalDevice.LogicalDevice,
			p_render->SwapChain.VkSwapchainKHR,
			99999999,
			l_synchronizationObject.ImageAvailableSemaphore,
			VK_NULL_HANDLE,
			&l_imageIndex);

		if (l_acquireNextImageResult == VK_ERROR_OUT_OF_DATE_KHR || l_acquireNextImageResult == VK_SUBOPTIMAL_KHR || p_render->SwapChain.MustBeRebuilt)
		{
			Render_recreateSwapChain(p_render);
			return;
		}
		else if (l_acquireNextImageResult != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to acquire swap chain image!"));
		}

		std::vector<SwapChainImage>* l_swapChainImages = &p_render->SwapChain.SwapChainImages;

		VkCommandBuffer l_commandBuffer = p_render->SwapChain.SwapChainImages.at(l_imageIndex).CommandBuffer.CommandBuffer;

		VkCommandBufferBeginInfo l_commandBufferBeginInfo{};
		l_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		l_commandBufferBeginInfo.flags = 0;
		l_commandBufferBeginInfo.pInheritanceInfo = nullptr;

		if (vkBeginCommandBuffer(l_commandBuffer, &l_commandBufferBeginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to begin recording command buffer!"));
		}

		CameraBufferSetupStep_buildCommandBuffer(&p_render->CameraBufferSetupStep, l_commandBuffer);

		DefaultMaterialDrawStep_buildCommandBuffer(&p_render->SwapChain, &p_render->DefaultMaterialDrawStep, l_commandBuffer, l_imageIndex);

		 IMGUITest_drawFrame(&p_render->IMGUITest, l_commandBuffer, l_imageIndex, &p_render->RenderInterface);

		if (vkEndCommandBuffer(l_commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to record command buffer!"));
		}

		VkSubmitInfo l_submitInfo{};
		l_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore l_waitSemaphore[] = { l_synchronizationObject.ImageAvailableSemaphore };
		VkPipelineStageFlags l_waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		l_submitInfo.waitSemaphoreCount = 1;
		l_submitInfo.pWaitSemaphores = l_waitSemaphore;
		l_submitInfo.pWaitDstStageMask = l_waitStages;
		l_submitInfo.commandBufferCount = 1;
		l_submitInfo.pCommandBuffers = &p_render->SwapChain.SwapChainImages[l_imageIndex].CommandBuffer.CommandBuffer;

		VkSemaphore l_signalSemaphores[] = { l_synchronizationObject.RenderFinishedSemaphore };
		l_submitInfo.signalSemaphoreCount = 1;
		l_submitInfo.pSignalSemaphores = l_signalSemaphores;

		if (vkQueueSubmit(p_render->Device.LogicalDevice.Queues.GraphicsQueue, 1, &l_submitInfo, l_synchronizationObject.WaitForGraphicsQueueFence) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to submit draw command buffer!"));
		}

		VkPresentInfoKHR l_presentInfo{ };
		l_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		l_presentInfo.waitSemaphoreCount = 1;
		l_presentInfo.pWaitSemaphores = l_signalSemaphores;

		VkSwapchainKHR l_swapChains[] = { p_render->SwapChain.VkSwapchainKHR };
		l_presentInfo.swapchainCount = 1;
		l_presentInfo.pSwapchains = l_swapChains;
		l_presentInfo.pImageIndices = &l_imageIndex;

		l_presentInfo.pResults = nullptr;

		vkQueuePresentKHR(p_render->Device.LogicalDevice.Queues.PresentQueue, &l_presentInfo);

		vkWaitForFences(p_render->Device.LogicalDevice.LogicalDevice, 1, &l_synchronizationObject.WaitForGraphicsQueueFence, VK_FALSE, UINT64_MAX);
	};



} // namespace _GameEngine