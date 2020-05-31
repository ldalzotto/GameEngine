#include "DescriptorSetLayout.h"

#include <stdexcept>
#include "Log/Log.h"

#include "Render/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	void DescriptorSetLayout_alloc(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device, DescriptorSetLayoutAllocInfo* p_descriptorSetLayoutAllocInfo)
	{
		p_descriptorSetLayout->LayoutBindings = *p_descriptorSetLayoutAllocInfo->LayoutBindings;

		VkDescriptorSetLayoutCreateInfo l_descriptorSetLayoutCreateInfo{};
		l_descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		l_descriptorSetLayoutCreateInfo.bindingCount = p_descriptorSetLayoutAllocInfo->LayoutBindings->size();
		l_descriptorSetLayoutCreateInfo.pBindings = p_descriptorSetLayoutAllocInfo->LayoutBindings->data();

		if (vkCreateDescriptorSetLayout(p_device->LogicalDevice.LogicalDevice, &l_descriptorSetLayoutCreateInfo, nullptr, &p_descriptorSetLayout->DescriptorSetLayout)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create descriptor set layout!"));
		}
	};

	void DescriptorSetLayout_free(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device)
	{
		vkDestroyDescriptorSetLayout(p_device->LogicalDevice.LogicalDevice, p_descriptorSetLayout->DescriptorSetLayout, nullptr);
	};
}