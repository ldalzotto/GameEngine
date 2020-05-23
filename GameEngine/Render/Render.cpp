#include "Render.h"

#include "Extensions/Extensions.h"
#include "Log/Log.h"

#include "Render/Mesh/Mesh.h"

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

	void initGraphicsPipeline(Render* p_render);
	void freeGraphicsPipeline(Render* p_render);

	void initCommandPool(Render* p_render);
	void freeCommandPool(Render* p_render);

	void initRenderSemaphore(Render* p_render);
	void freeRenderSemaphore(Render* p_render);

	_Mesh::Mesh DrawnMeshForTest;

	Render* alloc()
	{
		Render* l_render = new Render();

		Window_init(&l_render->Window);

		initValidationLayers(l_render);
		initializeVulkan(l_render);
		initVulkanDebugger(l_render);
		initSurface(l_render);
		initDevice(l_render);
		initCommandPool(l_render);
		initSwapChain(l_render);
		initGraphicsPipeline(l_render);
		initRenderSemaphore(l_render);

		StartRenderPassInfo l_startRenderPassInfo;
		l_startRenderPassInfo.CommandBuffers = &l_render->CommandBuffers;
		l_startRenderPassInfo.GraphicsPipeline = &l_render->GraphicsPipeline;
		l_startRenderPassInfo.SwapChain = &l_render->SwapChain;

		std::vector<_Mesh::Vertex> l_vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};
		std::vector<uint16_t> l_inidces = {
			 0, 1, 2, 2, 3, 0
		};

		_Mesh::MeshAllocInfo l_meshAllocInfo{};
		l_meshAllocInfo.Device = &l_render->Device;
		l_meshAllocInfo.Vertices = &l_vertices;
		l_meshAllocInfo.Indices = &l_inidces;
		_Mesh::Mesh_alloc(&DrawnMeshForTest, &l_meshAllocInfo);


		startRenderPass(&l_startRenderPassInfo);

		return l_render;
	};

	void free(Render** p_render)
	{
		// We wait for the Queues to finish their curent operation before releasing memory.
		// This is to ensure that no undefined behavior occurs while doing so.
		vkDeviceWaitIdle((*p_render)->Device.LogicalDevice.LogicalDevice);

		_Mesh::Mesh_free(&DrawnMeshForTest, &(*p_render)->Device);


		freeRenderSemaphore(*p_render);
		freeGraphicsPipeline(*p_render);
		freeSwapChain(*p_render);
		freeCommandPool(*p_render);
		freeDevice(*p_render);
		freeSurface(*p_render);
		freeVulkanDebugger((*p_render));
		freeVulkan(*p_render);
		delete* p_render;
		*p_render = nullptr;
	};

	void recreateSwapChain(Render* p_render)
	{
		// We wait for the Queues to finish their curent operation before releasing memory.
		// This is to ensure that no undefined behavior occurs while doing so.
		vkDeviceWaitIdle(p_render->Device.LogicalDevice.LogicalDevice);
		freeRenderSemaphore(p_render);
		freeGraphicsPipeline(p_render);
		freeSwapChain(p_render);
		freeCommandPool(p_render);

		initCommandPool(p_render);
		initSwapChain(p_render);
		initGraphicsPipeline(p_render);
		initRenderSemaphore(p_render);

		StartRenderPassInfo l_startRenderPassInfo;
		l_startRenderPassInfo.CommandBuffers = &p_render->CommandBuffers;
		l_startRenderPassInfo.GraphicsPipeline = &p_render->GraphicsPipeline;
		l_startRenderPassInfo.SwapChain = &p_render->SwapChain;
		startRenderPass(&l_startRenderPassInfo);

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
		_Extensions::checkPresenceOfRequiredInstanceExtensions(l_requiredExtensions);
		_Extensions::populateRequiredExtensions(&l_requiredExtensions, p_render->ValidationLayers.EnableValidationLayers);

		createInfo.enabledExtensionCount = static_cast<uint32_t>(l_requiredExtensions.size());
		createInfo.ppEnabledExtensionNames = l_requiredExtensions.data();

		// /!\ This messenger create info is used to log any problems in the creation and destroy of vulkan instance.
		//	   It is placed outside the if because vulakn only accepts a pointer to it, so this is to avoid stack destruction.
		VkDebugUtilsMessengerCreateInfoEXT l_debugUtlsMessengerCreateInfo{};

		_ValidationLayers::ValidationLayers* l_validationLayers = &p_render->ValidationLayers;
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
		_ValidationLayers::init(&p_render->ValidationLayers);
		_ValidationLayers::checkValidationLayerSupport(&p_render->ValidationLayers);
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
		_Surface::build(&p_render->WindowSurface, p_render->Instance, &p_render->Window);
	};

	void freeSurface(Render* p_render)
	{
		_Surface::release(&p_render->WindowSurface, p_render->Instance);
	};

	/////// END SURFACE

	/////// DEVICE

	void setupLogicalDeviceValidation(_ValidationLayers::ValidationLayers* p_validationLayers, VkDeviceCreateInfo* p_deviceCreateInfo);

	void initDevice(Render* p_render)
	{
		_Device::DeviceBuildInfo l_deviceBuildInfo{};
		l_deviceBuildInfo.Instance = p_render->Instance;

		_Device::DeviceBuildCallbacks* l_deviceBuildCallbacks = &l_deviceBuildInfo.DeviceBuildCallbacks;

		l_deviceBuildCallbacks->SetupValidation = [p_render](VkDeviceCreateInfo* p_deviceCreateInfo) {
			setupLogicalDeviceValidation(&p_render->ValidationLayers, p_deviceCreateInfo);
		};
		l_deviceBuildCallbacks->GetPhysicalDeviceSurfaceSupport = [p_render](VkPhysicalDevice p_device, uint32_t p_queueFamilyIndex, VkBool32* p_supported)
		{
			return vkGetPhysicalDeviceSurfaceSupportKHR(p_device, p_queueFamilyIndex, p_render->WindowSurface.WindowSurface, p_supported);
		};
		l_deviceBuildCallbacks->IsSwapChainSupported = [p_render](VkPhysicalDevice p_physicalDevice)
		{
			return _SwapChain::isSwapChainSupported(_SwapChain::getSwapChainSupportDetails(p_physicalDevice, &p_render->WindowSurface));
		};

		_Device::Device_build(&p_render->Device, &l_deviceBuildInfo);
	};

	void setupLogicalDeviceValidation(_ValidationLayers::ValidationLayers* p_validationLayers, VkDeviceCreateInfo* p_deviceCreateInfo)
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
		_Device::Device_free(&p_render->Device);
	};

	/////// END DEVICE

	/////// SWAP CHAIN

	void initSwapChain(Render* p_render)
	{
		_SwapChain::SwapChainBuildInfo l_swapChainBuildInfo{};
		l_swapChainBuildInfo.SwapChainDependencies.Device = &p_render->Device;
		l_swapChainBuildInfo.SwapChainDependencies.Surface = &p_render->WindowSurface;
		l_swapChainBuildInfo.SwapChainDependencies.Window = &p_render->Window;
		l_swapChainBuildInfo.CommandPool = &p_render->CommandPool;
		_SwapChain::build(&p_render->SwapChain, &l_swapChainBuildInfo);
	};

	void freeSwapChain(Render* p_render)
	{
		_SwapChain::swapChain_free(&p_render->SwapChain);
	};

	/////// END SWAP CHAIN


	/////// GRAPHICS PIPELINE

	void initGraphicsPipeline(Render* p_render)
	{
		_GraphicsPipeline::GraphicsPipelineDependencies l_graphicsPipelineDependencies{};
		l_graphicsPipelineDependencies.Device = &p_render->Device;
		l_graphicsPipelineDependencies.SwapChain = &p_render->SwapChain;
		_GraphicsPipeline::build(&p_render->GraphicsPipeline, l_graphicsPipelineDependencies);
	};

	void freeGraphicsPipeline(Render* p_render)
	{
		_GraphicsPipeline::GraphicsPipeline_free(&p_render->GraphicsPipeline);
	};

	/////// END GRAPHICS PIPELINE

	/////// COMMAND POOL

	void initCommandPool(Render* p_render)
	{
		_CommandBuffer::CommandPoolDependencies l_commandPoolDependencies{};
		l_commandPoolDependencies.Device = &p_render->Device;
		_CommandBuffer::CommandPool_Init(&p_render->CommandPool, &l_commandPoolDependencies);
	};

	void freeCommandPool(Render* p_render)
	{
		_CommandBuffer::CommandPool_free(&p_render->CommandPool);
	};

	/////// END COMMAND POOL

	/////// RENDER SEMAPHORE

	void initRenderSemaphore(Render* p_render)
	{
		_Synchronisation::RenderSemaphoreDependencies l_renderSemaphoreDependencies{ };
		l_renderSemaphoreDependencies.Device = &p_render->Device;

		_Synchronisation::RenderSemaphoreCreationInfo l_renderSemaphoreCreationInfo{};
		l_renderSemaphoreCreationInfo.MaxFramesInParallel = 2;
		l_renderSemaphoreCreationInfo.RenderSemaphoreDependencies = &l_renderSemaphoreDependencies;

		_Synchronisation::RenderSemaphore_init(&p_render->RenderSemaphore, &l_renderSemaphoreCreationInfo);
	};

	void freeRenderSemaphore(Render* p_render)
	{
		_Synchronisation::RenderSemaphore_free(&p_render->RenderSemaphore);
	};

	/////// END RENDER SEMAPHORE

	void startRenderPass(StartRenderPassInfo* p_startRenderPassInfo)
	{
		_GraphicsPipeline::RenderPass* RenderPass = &p_startRenderPassInfo->GraphicsPipeline->RenderPass;

		std::vector<_SwapChainImage::SwapChainImage>* l_swapChainImages = &p_startRenderPassInfo->SwapChain->SwapChainImages;
		std::vector<_GraphicsPipeline::FrameBuffer>* l_frameBuffers = &p_startRenderPassInfo->GraphicsPipeline->FrameBuffers;

		for (size_t i = 0; i < l_frameBuffers->size(); i++)
		{
			VkRenderPassBeginInfo l_renderPassBeginInfo{};
			l_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			l_renderPassBeginInfo.renderPass = RenderPass->renderPass;
			l_renderPassBeginInfo.framebuffer = l_frameBuffers->at(i).FrameBuffer;
			l_renderPassBeginInfo.renderArea.offset = { 0,0 };
			l_renderPassBeginInfo.renderArea.extent = p_startRenderPassInfo->SwapChain->SwapChainInfo.SwapExtend;

			VkClearValue l_clearValue = { 0.0f,0.0f,0.0f,1.0f };
			l_renderPassBeginInfo.clearValueCount = 1;
			l_renderPassBeginInfo.pClearValues = &l_clearValue;

			VkCommandBuffer l_commandBuffer = l_swapChainImages->at(i).CommandBuffer.CommandBuffer;

			vkCmdBeginRenderPass(l_commandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(l_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_startRenderPassInfo->GraphicsPipeline->Pipeline);

			VkBuffer l_vertexBuffers[] = { DrawnMeshForTest.VertexBuffer.Buffer };
			VkDeviceSize l_offsets[] = { 0 };
			vkCmdBindVertexBuffers(l_commandBuffer, 0, 1, l_vertexBuffers, l_offsets);
			vkCmdBindIndexBuffer(l_commandBuffer, DrawnMeshForTest.IndicesBuffer.Buffer, 0, VK_INDEX_TYPE_UINT16);

			// vkCmdDraw(l_commandBuffer, DrawnMeshForTest.Vertices.size(), 1, 0, 0);
			vkCmdDrawIndexed(l_commandBuffer, DrawnMeshForTest.Indices.size(), 1, 0, 0, 0);
			vkCmdEndRenderPass(l_commandBuffer);

			if (vkEndCommandBuffer(l_commandBuffer) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to record command buffer!"));
			}
		}
	};

	void render(Render* p_render)
	{
		_Synchronisation::RenderSemaphore_incrementFrameCount(&p_render->RenderSemaphore);
		_Synchronisation::CurrentSynchronisationObject l_synchronizationObject = _Synchronisation::RenderSemaphore_getCurrentSynchronisationObject(&p_render->RenderSemaphore);

		vkResetFences(p_render->Device.LogicalDevice.LogicalDevice, 1, &l_synchronizationObject.WaitForGraphicsQueueFence);

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
			recreateSwapChain(p_render);
			return;
		}
		else if (l_acquireNextImageResult != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to acquire swap chain image!"));
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