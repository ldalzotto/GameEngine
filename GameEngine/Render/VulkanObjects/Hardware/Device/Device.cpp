
#include "Device.h"
#include "MYLog/MYLog.h"
#include "VulkanObjects/Extensions/Extensions.h"
#include "Queue.h"

#include <stdexcept>
#include <vector>
#include <set>

namespace _GameEngine::_Render
{
	std::vector<char*> DeviceExtensions = std::vector<char*>{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		// This is to allow the viewport to be flipped
		VK_KHR_MAINTENANCE1_EXTENSION_NAME
	};

	void buildPhysicalDevice(Device* p_device, DeviceBuildInfo* p_deviceBuildInfo);
	void buildLogicalDevice(Device* p_device, DeviceBuildInfo* p_deviceBuildInfo);

	void Device_build(Device* p_device, DeviceBuildInfo* p_deviceBuildInfo)
	{
		buildPhysicalDevice(p_device, p_deviceBuildInfo);
		buildLogicalDevice(p_device, p_deviceBuildInfo);
	};

	void Device_free(Device* p_device)
	{
		vkDestroyDevice(p_device->LogicalDevice.LogicalDevice, nullptr);
	};

	bool isPhysicalDeviceElligible(VkPhysicalDevice l_physicalDevice, QueueFamilies* out_queueFamilies, DeviceBuildCallbacks* p_deviceBuildCallbacks)
	{
		return Queue_findDeviceQueues(l_physicalDevice, out_queueFamilies, p_deviceBuildCallbacks->GetPhysicalDeviceSurfaceSupport) &&
			Extensions_checkPresenceOfRequiredDeviceExtensions(DeviceExtensions, l_physicalDevice) &&
			p_deviceBuildCallbacks->IsSwapChainSupported(l_physicalDevice);
	}

	void buildPhysicalDevice(Device* p_device, DeviceBuildInfo* p_deviceBuildInfo)
	{
		p_device->PhysicalDevice.PhysicalDevice = VK_NULL_HANDLE;

		uint32_t l_deviceCount = 0;
		vkEnumeratePhysicalDevices(p_deviceBuildInfo->Instance, &l_deviceCount, nullptr);

		if (l_deviceCount == 0)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to find GPUs with Vulkan support!"));
		}

		std::vector<VkPhysicalDevice> l_physicalDevices(l_deviceCount);
		vkEnumeratePhysicalDevices(p_deviceBuildInfo->Instance, &l_deviceCount, l_physicalDevices.data());

		for (auto&& l_physicalDevice : l_physicalDevices)
		{
			if (isPhysicalDeviceElligible(l_physicalDevice,
				&p_device->PhysicalDevice.QueueFamilies, &p_deviceBuildInfo->DeviceBuildCallbacks))
			{
				p_device->PhysicalDevice.PhysicalDevice = l_physicalDevice;
				vkGetPhysicalDeviceFeatures(l_physicalDevice, &p_device->PhysicalDevice.DeviceAvailableFeatures);
				break;
			}
		}

		if (p_device->PhysicalDevice.PhysicalDevice == VK_NULL_HANDLE)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to find a suitable GPU!"));
		}
	};

	void buildLogicalDevice(Device* p_device, DeviceBuildInfo* p_deviceBuildInfo)
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
		l_deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
		l_deviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();


		VkPhysicalDeviceFeatures deviceFeatures{};
		if (p_device->PhysicalDevice.DeviceAvailableFeatures.samplerAnisotropy)
		{
			deviceFeatures.samplerAnisotropy = VK_TRUE;
		}

		l_deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		if (p_deviceBuildInfo->DeviceBuildCallbacks.SetupValidation)
		{
			p_deviceBuildInfo->DeviceBuildCallbacks.SetupValidation(&l_deviceCreateInfo);
		}

		if (vkCreateDevice(p_device->PhysicalDevice.PhysicalDevice, &l_deviceCreateInfo, nullptr, &p_device->LogicalDevice.LogicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create logical device!"));
		}

		vkGetDeviceQueue(p_device->LogicalDevice.LogicalDevice, p_device->PhysicalDevice.QueueFamilies.Graphics.QueueIndex, 0, &p_device->LogicalDevice.Queues.GraphicsQueue);
		vkGetDeviceQueue(p_device->LogicalDevice.LogicalDevice, p_device->PhysicalDevice.QueueFamilies.Present.QueueIndex, 0, &p_device->LogicalDevice.Queues.PresentQueue);
	};


	uint32_t Device_findMemoryType(Device* p_device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties l_physicalDeviceMemoryproperties{};
		vkGetPhysicalDeviceMemoryProperties(p_device->PhysicalDevice.PhysicalDevice, &l_physicalDeviceMemoryproperties);

		for (uint32_t i = 0; i < l_physicalDeviceMemoryproperties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (l_physicalDeviceMemoryproperties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}

		throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to find suitable memory type!"));
	};

	bool Device_isFormatSupported(Device* p_device, FormatSupportKey* p_formatSupportKey)
	{
		size_t p_formatSupportHash = _CoreV3::Hash(p_formatSupportKey);

		if (!p_device->PhysicalDevice.ImageFormatSupportCache.contains(p_formatSupportHash))
		{
			if (p_formatSupportKey->ImageTiling == VK_IMAGE_TILING_OPTIMAL)
			{
				VkFormatProperties l_formatProperties{};
				vkGetPhysicalDeviceFormatProperties(p_device->PhysicalDevice.PhysicalDevice, p_formatSupportKey->Format, &l_formatProperties);
				p_device->PhysicalDevice.ImageFormatSupportCache[p_formatSupportHash] = l_formatProperties.optimalTilingFeatures & p_formatSupportKey->FormatFeature;
			}
			else if (p_formatSupportKey->ImageTiling == VK_IMAGE_TILING_LINEAR)
			{
				VkFormatProperties l_formatProperties{};
				vkGetPhysicalDeviceFormatProperties(p_device->PhysicalDevice.PhysicalDevice, p_formatSupportKey->Format, &l_formatProperties);
				p_device->PhysicalDevice.ImageFormatSupportCache[p_formatSupportHash] = l_formatProperties.linearTilingFeatures & p_formatSupportKey->FormatFeature;
			}

		}

		return p_device->PhysicalDevice.ImageFormatSupportCache[p_formatSupportHash];
	};
}

namespace _CoreV3
{
	template <>
	size_t Hash<_GameEngine::_Render::FormatSupportKey>(_GameEngine::_Render::FormatSupportKey* p_formatSupportKey)
	{
		return HashCombine(0, &p_formatSupportKey->Format, &p_formatSupportKey->FormatFeature, &p_formatSupportKey->ImageTiling);
	};
}