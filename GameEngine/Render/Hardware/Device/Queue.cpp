#include "Queue.h"

#include <vector>

namespace _GameEngine::_Render::_Hardware::_Device
{

	void initQueueFamily(QueueFamily& p_queueFamily)
	{
		p_queueFamily.QueueFound = false;
		p_queueFamily.QueueIndex = 0;
	}

	bool getQueue(VkPhysicalDevice p_physicalDevice,
		QueueFamilies* out_queueFamilies,
		std::function<void(VkPhysicalDevice, uint32_t p_queueFamilyIndex, VkBool32*)> p_surfaceSupport)
	{
		initQueueFamily(out_queueFamilies->Graphics);
		initQueueFamily(out_queueFamilies->Present);

		uint32_t l_queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(p_physicalDevice, &l_queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> l_queueFamilies(l_queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(p_physicalDevice, &l_queueFamilyCount, l_queueFamilies.data());

		for (int i = 0; i < l_queueFamilies.size(); i++)
		{
			if (l_queueFamilies[i].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
			{
				out_queueFamilies->Graphics.QueueFound = true;
				out_queueFamilies->Graphics.QueueIndex = i;
			}

			VkBool32 l_presentSupport;
			p_surfaceSupport(p_physicalDevice, i, &l_presentSupport);
			if (l_presentSupport)
			{
				out_queueFamilies->Present.QueueFound = true;
				out_queueFamilies->Present.QueueIndex = i;
			}

			if (out_queueFamilies->Graphics.QueueFound && out_queueFamilies->Present.QueueFound)
			{
				return true;
			}
		}


		return false;
	};
}