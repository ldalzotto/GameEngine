
#include "Device.h"
#include "Log/Log.h"
#include "Render/Extensions/Extensions.h"
#include "Queue.h"

#include <stdexcept>
#include <vector>
#include <set>

namespace _GameEngine::_Render::_Device
{
	std::vector<char*> _Device::DeviceExtensions = std::vector<char*>{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	void buildPhysicalDevice(Device* p_device, const VkInstance& p_instance, DeviceBuildCallbacks* p_deviceBuildCallbacks);
	void buildLogicalDevice(Device* p_device, DeviceBuildCallbacks* p_deviceBuildCallbacks);

	void build(VkInstance p_instance, Device* p_device, DeviceBuildCallbacks* p_proxyCallbacks)
	{
		buildPhysicalDevice(p_device, p_instance, p_proxyCallbacks);
		buildLogicalDevice(p_device, p_proxyCallbacks);
	};

	void Device_free(Device* p_device)
	{
		vkDestroyDevice(p_device->LogicalDevice.LogicalDevice, nullptr);
	};

	bool isPhysicalDeviceElligible(VkPhysicalDevice l_physicalDevice, QueueFamilies* out_queueFamilies, DeviceBuildCallbacks* p_deviceBuildCallbacks)
	{
		return getQueue(l_physicalDevice, out_queueFamilies, p_deviceBuildCallbacks->GetPhysicalDeviceSurfaceSupport) &&
			_Extensions::checkPresenceOfRequiredDeviceExtensions(_Device::DeviceExtensions, l_physicalDevice) &&
			p_deviceBuildCallbacks->IsSwapChainSupported(l_physicalDevice);
	}

	void buildPhysicalDevice(Device* p_device, const VkInstance& p_instance, DeviceBuildCallbacks* p_deviceBuildCallbacks)
	{
		p_device->PhysicalDevice.PhysicalDevice = VK_NULL_HANDLE;

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
			if (isPhysicalDeviceElligible(l_physicalDevice, 
					&p_device->PhysicalDevice.QueueFamilies, p_deviceBuildCallbacks))
			{
				p_device->PhysicalDevice.PhysicalDevice = l_physicalDevice;
				break;
			}
		}

		if (p_device->PhysicalDevice.PhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to find a suitable GPU!"));
		}
	};

	void buildLogicalDevice(Device* p_device, DeviceBuildCallbacks* p_deviceBuildCallbacks)
	{
		std::set<uint32_t> l_uniqueDeviceQueueFamilyIndex{ p_device->PhysicalDevice.QueueFamilies.Graphics.QueueIndex, p_device->PhysicalDevice.QueueFamilies.Present.QueueIndex };
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
		l_deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(l_graphicsQueueCreationArray.size());
		l_deviceCreateInfo.pQueueCreateInfos = l_graphicsQueueCreationArray.data();
		l_deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(_Device::DeviceExtensions.size());
		l_deviceCreateInfo.ppEnabledExtensionNames = _Device::DeviceExtensions.data();

		VkPhysicalDeviceFeatures deviceFeatures{};
		l_deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		if (p_deviceBuildCallbacks->SetupValidation)
		{
			p_deviceBuildCallbacks->SetupValidation(&l_deviceCreateInfo);
		}

		if (vkCreateDevice(p_device->PhysicalDevice.PhysicalDevice, &l_deviceCreateInfo, nullptr, &p_device->LogicalDevice.LogicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create logical device!"));
		}

		vkGetDeviceQueue(p_device->LogicalDevice.LogicalDevice, p_device->PhysicalDevice.QueueFamilies.Graphics.QueueIndex, 0, &p_device->LogicalDevice.Queues.GraphicsQueue);
		vkGetDeviceQueue(p_device->LogicalDevice.LogicalDevice, p_device->PhysicalDevice.QueueFamilies.Present.QueueIndex, 0, &p_device->LogicalDevice.Queues.PresentQueue);
	};

}
