#include "Render.h"

#include "Extensions/Extensions.h"
#include "Log/Log.h"

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

	void initCommandBuffers(Render* p_render);

	Render* alloc()
	{
		Render* l_render = new Render();

		Window_init(&l_render->Window);

		initValidationLayers(l_render);
		initializeVulkan(l_render);
		initVulkanDebugger(l_render);
		initSurface(l_render);
		initDevice(l_render);
		initSwapChain(l_render);
		initGraphicsPipeline(l_render);
		initCommandPool(l_render);
		initCommandBuffers(l_render);


		StartRenderPassInfo l_startRenderPassInfo;
		l_startRenderPassInfo.CommandBuffers = &l_render->CommandBuffers;
		l_startRenderPassInfo.GraphicsPipeline = &l_render->GraphicsPipeline;
		l_startRenderPassInfo.SwapChain = &l_render->SwapChain;
		startRenderPass(&l_startRenderPassInfo);

		return l_render;
	};

	void free(Render** p_render)
	{
		freeCommandPool(*p_render);
		freeGraphicsPipeline(*p_render);
		freeSwapChain(*p_render);
		freeDevice(*p_render);
		freeSurface(*p_render);
		freeVulkanDebugger((*p_render));
		freeVulkan(*p_render);
		delete* p_render;
		*p_render = nullptr;
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
		_SwapChain::SwapChainDependencies l_swapChainCreation{};
		l_swapChainCreation.Device = &p_render->Device;
		l_swapChainCreation.Surface = &p_render->WindowSurface;
		l_swapChainCreation.Window = &p_render->Window;
		_SwapChain::build(&p_render->SwapChain, l_swapChainCreation);
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

	/////// COMMAND BUFFERS

	void initCommandBuffers(Render* p_render)
	{
		_CommandBuffer::CommandBuffersDependencies l_commandBuffersDependencies{};
		l_commandBuffersDependencies.CommandPool = &p_render->CommandPool;
		l_commandBuffersDependencies.FrameBuffer = &p_render->GraphicsPipeline.FrameBuffer;
		_CommandBuffer::CommandBuffers_init(&p_render->CommandBuffers, &l_commandBuffersDependencies);
	};

	/////// END COMMAND BUFFERS

	void startRenderPass(StartRenderPassInfo* p_startRenderPassInfo)
	{
		_GraphicsPipeline::RenderPass* RenderPass = &p_startRenderPassInfo->GraphicsPipeline->RenderPass;
		_GraphicsPipeline::FrameBuffer* FrameBuffer = &p_startRenderPassInfo->GraphicsPipeline->FrameBuffer;

		for (size_t i = 0; i < FrameBuffer->FrameBuffers.size(); i++)
		{
			VkRenderPassBeginInfo l_renderPassBeginInfo{};
			l_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			l_renderPassBeginInfo.renderPass = RenderPass->renderPass;
			l_renderPassBeginInfo.framebuffer = FrameBuffer->FrameBuffers[i];
			l_renderPassBeginInfo.renderArea.offset = { 0,0 };
			l_renderPassBeginInfo.renderArea.extent = p_startRenderPassInfo->SwapChain->SwapChainInfo.SwapExtend;

			VkClearValue l_clearValue = { 0.0f,0.0f,0.0f,1.0f };
			l_renderPassBeginInfo.clearValueCount = 1;
			l_renderPassBeginInfo.pClearValues = &l_clearValue;

			VkCommandBuffer l_commandBuffer = p_startRenderPassInfo->CommandBuffers->CommandBuffers[i];

			vkCmdBeginRenderPass(l_commandBuffer, &l_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(l_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_startRenderPassInfo->GraphicsPipeline->Pipeline);
			vkCmdDraw(l_commandBuffer, 3, 1, 0, 0);
			vkCmdEndRenderPass(l_commandBuffer);

			if (vkEndCommandBuffer(l_commandBuffer) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to record command buffer!"));
			}
		}
	};

	void render(Render* p_render)
	{

	};

	

} // namespace _GameEngine