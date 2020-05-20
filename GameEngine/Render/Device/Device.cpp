
#include "Device.h"
#include "Log/Log.h"

#include <stdexcept>
#include <vector>

namespace _GameEngine::_Render::_Device
{
	struct QueueFamily
	{
		bool QueueFound;
		uint32_t QueueIndex;
	};

	void QueueFamily_build(QueueFamily* p_queueFamily, bool p_queueFound, uint32_t p_queueIndex)
	{
		p_queueFamily->QueueFound = p_queueFound;
		p_queueFamily->QueueIndex = p_queueIndex;
	};

	struct QueueFamilies
	{
		QueueFamily Graphics;
	};

	void QueueFamilies_build(QueueFamilies* p_queueFamilies)
	{
		QueueFamily_build(&p_queueFamilies->Graphics, false, 0);
	};

	QueueFamilies findQueues(VkPhysicalDevice p_physicalDevice)
	{
		QueueFamilies l_queues;
		QueueFamilies_build(&l_queues);

		uint32_t l_queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(p_physicalDevice, &l_queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> l_queueFamilies(l_queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(p_physicalDevice, &l_queueFamilyCount, l_queueFamilies.data());

		for (int i = 0; i < l_queueFamilies.size(); i++)
		{
			if(l_queueFamilies[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
			{
				QueueFamily_build(&l_queues.Graphics, true, i);
				break;
			}
		}

		return l_queues;
	};

	bool isPhysicalDeviceElligible(VkPhysicalDevice l_physicalDevice)
	{
		return findQueues(l_physicalDevice).Graphics.QueueFound;
	}

	void Device_build(VkInstance p_instance, Device* p_device)
	{
		p_device->PhysicalDevice = VK_NULL_HANDLE;

		uint32_t l_deviceCount = 0;
		vkEnumeratePhysicalDevices(p_instance, &l_deviceCount, nullptr);

		if (l_deviceCount == 0)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to find GPUs with Vulkan support!"));
		}

		std::vector<VkPhysicalDevice> l_physicalDevices(l_deviceCount);
		vkEnumeratePhysicalDevices(p_instance, &l_deviceCount, l_physicalDevices.data());

		for (auto&& l_physicalDevice : l_physicalDevices)
		{
			if (isPhysicalDeviceElligible(l_physicalDevice))
			{
				p_device->PhysicalDevice = l_physicalDevice;
				break;
			}
		}

		if (p_device->PhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to find a suitable GPU!"));
		}
	};
}
