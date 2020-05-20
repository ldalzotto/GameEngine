#pragma once

#include "vulkan/vulkan.h"

#include <vector>
#include <functional>

namespace _GameEngine::_Render::_Device
{
	extern std::vector<char*> DeviceExtensions;

	struct Device 
	{
		VkPhysicalDevice PhysicalDevice;
		VkDevice LogicalDevice;
		VkQueue GraphicsQueue;
		VkQueue PresentQueue;
	};

	struct DeviceValidationLayer
	{
		void* Closure;
		void(*SetupValidation)(DeviceValidationLayer*, VkDeviceCreateInfo*);
	};

	struct QueueQueries
	{
		void* PROXY_vkGetPhysicalDeviceSurfaceSupportKHR_closure;
		VkResult(*PROXY_vkGetPhysicalDeviceSurfaceSupportKHR)(QueueQueries* p_closure, VkPhysicalDevice p_device, uint32_t p_queueFamilyIndex, VkBool32* p_supported);
	};

	struct SwapChainQuery
	{
		std::function<bool(VkPhysicalDevice PhysicalDevice)> IsSwapChainSupported;
	};

	struct DeviceBuildPROXYCallbacks 
	{
		DeviceValidationLayer DeviceValidation;
		QueueQueries QueueQueries;
		SwapChainQuery SwapChainQuery;
	};
		
	void Device_build(VkInstance p_instance, Device* p_device, DeviceBuildPROXYCallbacks* p_proxyCallbacks);

	void Device_free(Device* p_device);
}