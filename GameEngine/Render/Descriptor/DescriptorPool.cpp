#include "DescriptorPool.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render
{
	void DescriptorPool_buildUnique(DescriptorPool* p_descriptorPool, Device* p_device, DescriptorPoolBuildInfo* l_descriptoPoolBuildInfo)
	{		
		VkDescriptorPoolCreateInfo l_descriptorPoolCreateInfo{};
		l_descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		l_descriptorPoolCreateInfo.flags = l_descriptoPoolBuildInfo->DescriptionPoolCreateFlags;
		l_descriptorPoolCreateInfo.poolSizeCount = 1;
		l_descriptorPoolCreateInfo.pPoolSizes = (const VkDescriptorPoolSize*) l_descriptoPoolBuildInfo->DescriptorPoolSizes->data();
		l_descriptorPoolCreateInfo.maxSets = l_descriptoPoolBuildInfo->MaxSet;

		if (vkCreateDescriptorPool(p_device->LogicalDevice.LogicalDevice, &l_descriptorPoolCreateInfo, nullptr, &p_descriptorPool->DescriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create the descriptor pool."));
		}
	};

	void DescriptorPool_freeUnique(DescriptorPool* p_descriptorPool, Device* p_device)
	{
		vkDestroyDescriptorPool(p_device->LogicalDevice.LogicalDevice, p_descriptorPool->DescriptorPool, nullptr);
	};
}