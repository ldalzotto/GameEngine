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

	struct DeviceBuildPROXYCallbacks 
	{
		std::function<void(VkDeviceCreateInfo*)> SetupValidation;
		std::function<VkResult(VkPhysicalDevice p_device, uint32_t p_queueFamilyIndex, VkBool32* p_supported)> GetPhysicalDeviceSurfaceSupport;
		std::function<bool(VkPhysicalDevice PhysicalDevice)> IsSwapChainSupported;
	};
		
	void Device_build(VkInstance p_instance, Device* p_device, DeviceBuildPROXYCallbacks* p_proxyCallbacks);

	void Device_free(Device* p_device);
}