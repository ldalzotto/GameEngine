#include "Render.h"

#include "Extensions/Extensions.h"
#include "_Composition/SwapChainComposition.h"
#include "Log/Log.h"

#include <stdexcept>


namespace _GameEngine::_Render
{
	void initializeVulkan(Render* p_render);
	void freeVulkan(Render* p_render);

	void initVulkanDebugger(Render* p_render);
	void initVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT* p_debugUtilsMessengerCreateInfo);
	void freeVulkanDebugger(Render* p_render);

	void setupLogicalDeviceValidation(_Device::DeviceValidationLayer* p_deviceValidation, VkDeviceCreateInfo* p_deviceCreateInfo);
	VkResult PROXY_vkGetPhysicalDeviceSurfaceSupportKHR(_Device::QueueQueries* p_closure, VkPhysicalDevice p_physicalDevice, uint32_t p_queueFamilyIndex, VkBool32* p_supported);

	Render* alloc()
	{
		Render* l_render = new Render();

		Window_init(&l_render->Window);
		initializeVulkan(l_render);

		return l_render;
	};

	void free(Render** p_render)
	{
		_Device::Device_free(&(*p_render)->Device);
		_Surface::release(&(*p_render)->WindowSurface, (*p_render)->Instance);
		freeVulkan(*p_render);
		delete* p_render;
		*p_render = nullptr;
	};

	void initializeVulkan(Render* p_render)
	{
		_ValidationLayers::init(&p_render->ValidationLayers);
		_ValidationLayers::checkValidationLayerSupport(&p_render->ValidationLayers);

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

		createInfo.enabledExtensionCount = l_requiredExtensions.size();
		createInfo.ppEnabledExtensionNames = l_requiredExtensions.data();

		// /!\ This messenger create info is used to log any problems in the creation and destroy of vulkan instance.
		//	   It is placed outside the if because vulakn only accepts a pointer to it, so this is to avoid stack destruction.
		VkDebugUtilsMessengerCreateInfoEXT l_debugUtlsMessengerCreateInfo{};

		_ValidationLayers::ValidationLayers* l_validationLayers = &p_render->ValidationLayers;
		if (l_validationLayers->EnableValidationLayers)
		{
			createInfo.enabledLayerCount = l_validationLayers->ValidationLayers.size();
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

		initVulkanDebugger(p_render);

		_Surface::build(&p_render->WindowSurface, p_render->Instance, &p_render->Window);

		_Device::DeviceBuildPROXYCallbacks l_deviceBuildPROXYCallbacks{};

		l_deviceBuildPROXYCallbacks.DeviceValidation.Closure = &p_render->ValidationLayers;
		l_deviceBuildPROXYCallbacks.DeviceValidation.SetupValidation = setupLogicalDeviceValidation;

		l_deviceBuildPROXYCallbacks.QueueQueries.PROXY_vkGetPhysicalDeviceSurfaceSupportKHR = PROXY_vkGetPhysicalDeviceSurfaceSupportKHR;
		l_deviceBuildPROXYCallbacks.QueueQueries.PROXY_vkGetPhysicalDeviceSurfaceSupportKHR_closure = &p_render->WindowSurface;

		l_deviceBuildPROXYCallbacks.SwapChainQuery.IsSwapChainSupported = [p_render](VkPhysicalDevice PhysicalDevice) {
			return _SwapChain::_Composition::isSwapChainSupported(&p_render->WindowSurface, PhysicalDevice);
		};
		
		_Device::Device_build(p_render->Instance, &p_render->Device, &l_deviceBuildPROXYCallbacks);
	}

	void freeVulkan(Render* p_render)
	{
		freeVulkanDebugger(p_render);
		vkDestroyInstance(p_render->Instance, nullptr);
	}

	void render(Render* p_render)
	{

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

	void setupLogicalDeviceValidation(_Device::DeviceValidationLayer* p_deviceValidation, VkDeviceCreateInfo* p_deviceCreateInfo)
	{
		auto l_validationLayers = (_ValidationLayers::ValidationLayers*)p_deviceValidation->Closure;
		if (l_validationLayers->EnableValidationLayers)
		{
			p_deviceCreateInfo->enabledLayerCount = l_validationLayers->ValidationLayers.size();
			p_deviceCreateInfo->ppEnabledLayerNames = l_validationLayers->ValidationLayers.data();
		}
		else
		{
			p_deviceCreateInfo->enabledLayerCount = 0;
		}
	};

	VkResult PROXY_vkGetPhysicalDeviceSurfaceSupportKHR(_Device::QueueQueries* p_closure, VkPhysicalDevice p_physicalDevice, uint32_t p_queueFamilyIndex, VkBool32* p_supported)
	{
		_Surface::Surface* l_surface = (_Surface::Surface*)p_closure->PROXY_vkGetPhysicalDeviceSurfaceSupportKHR_closure;
		return vkGetPhysicalDeviceSurfaceSupportKHR(p_physicalDevice, p_queueFamilyIndex, l_surface->WindowSurface, p_supported);
	};



} // namespace _GameEngine