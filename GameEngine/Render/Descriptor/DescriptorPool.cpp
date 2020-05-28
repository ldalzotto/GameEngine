#include "DescriptorPool.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render
{
	void DescriptorPool_buildUnique(DescriptorPool* p_descriptorPool, Device* p_device, DescriptorPoolBuildInfo* l_descriptoPoolBuildInfo)
	{
		VkDescriptorPoolSize l_descriptorPoolSize{};
		l_descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		// 1 is the number of objects we render, there is only one for now
		l_descriptorPoolSize.descriptorCount = l_descriptoPoolBuildInfo->MaxDescriptorCount;

		VkDescriptorPoolCreateInfo l_descriptorPoolCreateInfo{};
		l_descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		l_descriptorPoolCreateInfo.flags = l_descriptoPoolBuildInfo->DescriptionPoolCreateFlags;
		l_descriptorPoolCreateInfo.poolSizeCount = 1;
		l_descriptorPoolCreateInfo.pPoolSizes = &l_descriptorPoolSize;
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