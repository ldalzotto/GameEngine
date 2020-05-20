
#include "vulkan/vulkan.h"

#include <vector>

namespace _GameEngine::_Render::_Device
{
	struct Device 
	{
		VkPhysicalDevice PhysicalDevice;
		VkDevice LogicalDevice;
		VkQueue GraphicsQueue;
		VkQueue PresentQueue;
	};

	struct DeviceValidation
	{
		void* Closure;
		void(*SetupValidation)(DeviceValidation*, VkDeviceCreateInfo*);
	};

	struct QueueQueries
	{
		void* PROXY_vkGetPhysicalDeviceSurfaceSupportKHR_closure;
		VkResult(*PROXY_vkGetPhysicalDeviceSurfaceSupportKHR)(QueueQueries* p_closure, VkPhysicalDevice p_device, uint32_t p_queueFamilyIndex, VkBool32* p_supported);
	};
	
	void Device_build(VkInstance p_instance, Device* p_device, 
				QueueQueries* p_queueQueries,
				DeviceValidation* p_deviceValidation = nullptr);

	void Device_free(Device* p_device);
}