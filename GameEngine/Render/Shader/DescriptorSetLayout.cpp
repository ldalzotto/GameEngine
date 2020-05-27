#include "DescriptorSetLayout.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render
{
	void DescriptorSetLayout_alloc(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device, DescriptorSetLayoutAllocInfo* p_descriptorSetLayoutAllocInfo)
	{
		VkDescriptorSetLayoutBinding l_descriptorSetLayoutBinding{};
		l_descriptorSetLayoutBinding.binding = p_descriptorSetLayoutAllocInfo->Binding;
		l_descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorSetLayoutBinding.descriptorCount = 1;
		l_descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		l_descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo l_descriptorSetLayoutCreateInfo{};
		l_descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		l_descriptorSetLayoutCreateInfo.bindingCount = 1;
		l_descriptorSetLayoutCreateInfo.pBindings = &l_descriptorSetLayoutBinding;

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