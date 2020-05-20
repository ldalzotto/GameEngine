
#include "vulkan/vulkan.h"

#include <vector>

namespace _GameEngine::_Render::_Device
{
	struct Device 
	{
		VkPhysicalDevice PhysicalDevice;
		VkDevice LogicalDevice;
		VkQueue GraphicsQueue;
	};

	struct DeviceValidation
	{
		void* Closure;
		void(*SetupValidation)(DeviceValidation*, VkDeviceCreateInfo*);
	};
	
	void Device_build(VkInstance p_instance, Device* p_device, DeviceValidation* p_deviceValidation = nullptr);
	void Device_free(Device* p_device);
}