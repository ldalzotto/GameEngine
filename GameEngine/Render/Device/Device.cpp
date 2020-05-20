
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

	struct QueueFamilies
	{
		QueueFamily Graphics;
	};

	bool isPhysicalDeviceElligible(VkPhysicalDevice l_physicalDevice);
	QueueFamilies findQueues(VkPhysicalDevice p_physicalDevice);

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

		QueueFamilies l_physicalQueueFamilies = findQueues(p_device->PhysicalDevice);


		std::vector<float> l_graphicsQueuePriorityArray{ 1.0f };

		VkDeviceQueueCreateInfo l_graphicsQueueCreateInfo{};
		l_graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		l_graphicsQueueCreateInfo.flags = VkDeviceQueueCreateFlagBits::VK_DEVICE_QUEUE_CREATE_PROTECTED_BIT;
		l_graphicsQueueCreateInfo.queueFamilyIndex = l_physicalQueueFamilies.Graphics.QueueIndex;
		l_graphicsQueueCreateInfo.queueCount = 1;
		l_graphicsQueueCreateInfo.pQueuePriorities = l_graphicsQueuePriorityArray.data();

		std::vector<VkDeviceQueueCreateInfo> l_graphicsQueueCreationArray{ l_graphicsQueueCreateInfo };

		VkDeviceCreateInfo l_deviceCreateInfo{};
		l_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		l_deviceCreateInfo.queueCreateInfoCount = l_graphicsQueueCreationArray.size();
		l_deviceCreateInfo.pQueueCreateInfos = l_graphicsQueueCreationArray.data();

		vkCreateDevice(p_device->PhysicalDevice, &l_deviceCreateInfo, nullptr, &p_device->LogicalDevice);
	};

	void Device_free(Device* p_device)
	{
		vkDestroyDevice(p_device->LogicalDevice, nullptr);
	};

	bool isPhysicalDeviceElligible(VkPhysicalDevice l_physicalDevice)
	{
		return findQueues(l_physicalDevice).Graphics.QueueFound;
	}

	void QueueFamily_build(QueueFamily* p_queueFamily, bool p_queueFound, uint32_t p_queueIndex);
	void QueueFamilies_build(QueueFamilies* p_queueFamilies);

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
			if (l_queueFamilies[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
			{
				QueueFamily_build(&l_queues.Graphics, true, i);
				break;
			}
		}

		return l_queues;
	};

	void QueueFamily_build(QueueFamily* p_queueFamily, bool p_queueFound, uint32_t p_queueIndex)
	{
		p_queueFamily->QueueFound = p_queueFound;
		p_queueFamily->QueueIndex = p_queueIndex;
	};

	void QueueFamilies_build(QueueFamilies* p_queueFamilies)
	{
		QueueFamily_build(&p_queueFamilies->Graphics, false, 0);
	};

}
