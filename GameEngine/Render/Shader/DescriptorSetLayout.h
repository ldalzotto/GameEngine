#pragma once

#include "vulkan/vulkan.h"
#include "Render/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	struct DescriptorSetLayout
	{
		VkDescriptorSetLayout DescriptorSetLayout;
	};

	struct DescriptorSetLayoutAllocInfo
	{
		uint32_t Binding;
	};

	void DescriptorSetLayout_alloc(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device, DescriptorSetLayoutAllocInfo* p_descriptorSetLayoutAllocInfo);
	void DescriptorSetLayout_free(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device);
}