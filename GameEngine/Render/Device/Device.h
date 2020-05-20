
#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_Device
{
	struct Device 
	{
		VkPhysicalDevice PhysicalDevice;
	};
	
	void Device_build(VkInstance p_instance, Device* p_device);
}