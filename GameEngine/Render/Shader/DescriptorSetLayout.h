#pragma once

#include "Includes/VulkanIncludes.h"
#include <vector>

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct DescriptorSetLayout
	{
		VkDescriptorSetLayout DescriptorSetLayout;
		std::vector<VkDescriptorSetLayoutBinding> LayoutBindings;
	};

	struct DescriptorSetLayoutAllocInfo
	{
		std::vector<VkDescriptorSetLayoutBinding>* LayoutBindings;
	};

	void DescriptorSetLayout_alloc(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device,  DescriptorSetLayoutAllocInfo* p_descriptorSetLayoutAllocInfo);
	void DescriptorSetLayout_free(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device);
}