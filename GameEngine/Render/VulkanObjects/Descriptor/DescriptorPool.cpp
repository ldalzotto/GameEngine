#include "DescriptorPool.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include "Shader/DescriptorSetLayout.h"

#include "Shader/DescriptorSetLayout.h"

#include <stdexcept>
#include "Log/LogFormatting.hpp"

namespace _GameEngine::_Render
{
	void DescriptorPool_alloc(DescriptorPool* p_descriptorPool, Device* p_device, DescriptorPoolAllocInfo* p_descriptorPoolAllocInfo)
	{
		if (p_descriptorPoolAllocInfo->SourceDescriptorPoolSize->size() > 0)
		{
			VkDescriptorPoolCreateInfo l_descriptorPoolCreateInfo{};

			l_descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			l_descriptorPoolCreateInfo.flags = p_descriptorPoolAllocInfo->DescriptionPoolCreateFlags;
			l_descriptorPoolCreateInfo.poolSizeCount = 1;
			l_descriptorPoolCreateInfo.pPoolSizes = (const VkDescriptorPoolSize*)p_descriptorPoolAllocInfo->SourceDescriptorPoolSize->data();
			l_descriptorPoolCreateInfo.maxSets = p_descriptorPoolAllocInfo->MaxSet;

			if (vkCreateDescriptorPool(p_device->LogicalDevice.LogicalDevice, &l_descriptorPoolCreateInfo, nullptr, &p_descriptorPool->DescriptorPool) != VK_SUCCESS)
			{
				throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create the descriptor pool."));
			}
		}
	};

	void DescriptorPool_free(DescriptorPool* p_descriptorPool, Device* p_device)
	{
		vkDestroyDescriptorPool(p_device->LogicalDevice.LogicalDevice, p_descriptorPool->DescriptorPool, nullptr);
	};


	std::vector<VkDescriptorPoolSize> DescriptorPool_buildDescriptorPoolSizeFromDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout)
	{
		std::vector<VkDescriptorSetLayoutBinding>* l_descriptorSetLayoutBindings =	&p_descriptorSetLayout->LayoutBindings;
		std::vector<VkDescriptorPoolSize> l_descriptorPoolSizes(l_descriptorSetLayoutBindings->size());

		for (size_t i = 0; i < l_descriptorPoolSizes.size(); i++)
		{
			VkDescriptorSetLayoutBinding* l_descriptorSetLayoutBinding = &l_descriptorSetLayoutBindings->at(i);
			VkDescriptorPoolSize* l_descritptorPoolSize = &l_descriptorPoolSizes.at(i);

			l_descritptorPoolSize->descriptorCount = l_descriptorSetLayoutBinding->descriptorCount;
			l_descritptorPoolSize->type = l_descriptorSetLayoutBinding->descriptorType;
		}

		return l_descriptorPoolSizes;
	};
}