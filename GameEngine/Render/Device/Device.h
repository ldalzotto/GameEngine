#pragma once

#include "vulkan/vulkan.h"

#include "Queue.h"

#include <vector>
#include <functional>

namespace _GameEngine::_Render::_Device
{
	extern std::vector<char*> DeviceExtensions;

	/**
		@brief The Physical GPU that has been selected for rendering.
	*/
	struct PhysicalDevice
	{
		VkPhysicalDevice PhysicalDevice;
		QueueFamilies QueueFamilies;
	};

	struct LogicalDeviceQueues
	{
		VkQueue GraphicsQueue;
		VkQueue PresentQueue;
	};

	/**
		@brief The programmatical interface of the selected @see PhysicalDevice.
	*/
	struct LogicalDevice
	{
		VkDevice LogicalDevice;
		LogicalDeviceQueues Queues;
	};

	struct Device 
	{
		PhysicalDevice PhysicalDevice;
		LogicalDevice LogicalDevice;
	};

	struct DeviceBuildCallbacks 
	{
		std::function<void(VkDeviceCreateInfo*)> SetupValidation;
		std::function<VkResult(VkPhysicalDevice p_device, uint32_t p_queueFamilyIndex, VkBool32* p_supported)> GetPhysicalDeviceSurfaceSupport;
		std::function<bool(VkPhysicalDevice PhysicalDevice)> IsSwapChainSupported;
	};
		
	void build(VkInstance p_instance, Device* p_device, DeviceBuildCallbacks* p_proxyCallbacks);

	void Device_free(Device* p_device);
}