
#include "Device.h"
#include "Log/Log.h"
#include "Render/Extensions/Extensions.h"

#include <stdexcept>
#include <vector>
#include <set>

namespace _GameEngine::_Render::_Device
{
	std::vector<char*> _Device::DeviceExtensions = std::vector<char*>{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	struct QueueFamily
	{
		bool QueueFound;
		uint32_t QueueIndex;
	};

	struct QueueFamilies
	{
		QueueFamily Graphics;
		QueueFamily Present;
	};

	void Device_buildPhysicalDevice(Device* p_device, const VkInstance& p_instance, QueueQueries* p_queueQueries, SwapChainQuery* p_swapChainQuery);
	void Device_buildLogicalDevice(QueueFamilies& l_physicalQueueFamilies, DeviceValidationLayer* p_deviceValidation, Device* p_device);

	QueueFamilies findQueues(VkPhysicalDevice p_physicalDevice, QueueQueries* p_queueQueries);
	bool QueueFamilies_allQueuesFound(QueueFamilies* p_queueFamilies);

	
	void Device_build(VkInstance p_instance, Device* p_device, DeviceBuildPROXYCallbacks* p_proxyCallbacks)
	{
		Device_buildPhysicalDevice(p_device, p_instance, &p_proxyCallbacks->QueueQueries, &p_proxyCallbacks->SwapChainQuery);

		QueueFamilies l_physicalQueueFamilies = findQueues(p_device->PhysicalDevice, &p_proxyCallbacks->QueueQueries);

		Device_buildLogicalDevice(l_physicalQueueFamilies, &p_proxyCallbacks->DeviceValidation, p_device);

		vkGetDeviceQueue(p_device->LogicalDevice, l_physicalQueueFamilies.Graphics.QueueIndex, 0, &p_device->GraphicsQueue);
		vkGetDeviceQueue(p_device->LogicalDevice, l_physicalQueueFamilies.Present.QueueIndex, 0, &p_device->PresentQueue);
	};

	void Device_free(Device* p_device)
	{
		vkDestroyDevice(p_device->LogicalDevice, nullptr);
	};

	bool isPhysicalDeviceElligible(VkPhysicalDevice l_physicalDevice, QueueQueries* p_queueQueries, SwapChainQuery* p_swapChainQuery)
	{
		QueueFamilies l_queues = findQueues(l_physicalDevice, p_queueQueries);
		return 
				QueueFamilies_allQueuesFound(&l_queues) && 
				_Extensions::checkPresenceOfRequiredDeviceExtensions(_Device::DeviceExtensions, l_physicalDevice) &&
				p_swapChainQuery->IsSwapChainSupported(l_physicalDevice);
	}

	void Device_buildPhysicalDevice(Device* p_device, const VkInstance& p_instance, QueueQueries* p_queueQueries, SwapChainQuery* p_swapChainQuery)
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
			if (isPhysicalDeviceElligible(l_physicalDevice, p_queueQueries, p_swapChainQuery))
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

	void Device_buildLogicalDevice(QueueFamilies& l_physicalQueueFamilies, DeviceValidationLayer* p_deviceValidation, Device* p_device)
	{
		std::set<uint32_t> l_uniqueDeviceQueueFamilyIndex{ l_physicalQueueFamilies.Graphics.QueueIndex, l_physicalQueueFamilies.Present.QueueIndex };
		std::vector<float> l_graphicsQueuePriorityArray{ 1.0f };

		std::vector<VkDeviceQueueCreateInfo> l_graphicsQueueCreationArray;

		for (auto&& l_queueFamilyIndex : l_uniqueDeviceQueueFamilyIndex)
		{
			VkDeviceQueueCreateInfo l_graphicsQueueCreateInfo{};
			l_graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			l_graphicsQueueCreateInfo.queueFamilyIndex = l_queueFamilyIndex;
			l_graphicsQueueCreateInfo.queueCount = 1;
			l_graphicsQueueCreateInfo.pQueuePriorities = l_graphicsQueuePriorityArray.data();

			l_graphicsQueueCreationArray.emplace_back(l_graphicsQueueCreateInfo);
		}

		VkDeviceCreateInfo l_deviceCreateInfo{};
		l_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		l_deviceCreateInfo.queueCreateInfoCount = l_graphicsQueueCreationArray.size();
		l_deviceCreateInfo.pQueueCreateInfos = l_graphicsQueueCreationArray.data();
		l_deviceCreateInfo.enabledExtensionCount = _Device::DeviceExtensions.size();
		l_deviceCreateInfo.ppEnabledExtensionNames = _Device::DeviceExtensions.data();

		VkPhysicalDeviceFeatures deviceFeatures{};
		l_deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		if (p_deviceValidation)
		{
			p_deviceValidation->SetupValidation(p_deviceValidation, &l_deviceCreateInfo);
		}

		if (vkCreateDevice(p_device->PhysicalDevice, &l_deviceCreateInfo, nullptr, &p_device->LogicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create logical device!"));
		}
	};

	void QueueFamily_build(QueueFamily* p_queueFamily, bool p_queueFound, uint32_t p_queueIndex);
	void QueueFamilies_build(QueueFamilies* p_queueFamilies);

	QueueFamilies findQueues(VkPhysicalDevice p_physicalDevice, QueueQueries* p_queueQueries)
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
			}

			VkBool32 l_presentSupport;
			p_queueQueries->PROXY_vkGetPhysicalDeviceSurfaceSupportKHR(p_queueQueries, p_physicalDevice, i, &l_presentSupport);
			if (l_presentSupport)
			{
				QueueFamily_build(&l_queues.Present, true, i);
			}

			if (QueueFamilies_allQueuesFound(&l_queues))
			{
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

	bool QueueFamilies_allQueuesFound(QueueFamilies* p_queueFamilies)
	{
		return p_queueFamilies->Graphics.QueueFound && p_queueFamilies->Present.QueueFound;
	};

}
