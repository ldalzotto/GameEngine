#pragma once

#include <vector>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct DescriptorSetLayout;
}

namespace _GameEngine::_Render
{
	struct DescriptorPool
	{
		VkDescriptorPool DescriptorPool;
	};

	struct DescriptorPoolAllocInfo
	{
		uint32_t MaxSet;
		VkDescriptorPoolCreateFlags DescriptionPoolCreateFlags;
		std::vector<VkDescriptorPoolSize>* SourceDescriptorPoolSize;
	};

	void DescriptorPool_alloc(DescriptorPool* p_descriptorPool, Device* p_device, DescriptorPoolAllocInfo* p_descriptorPoolAllocInfo);
	void DescriptorPool_free(DescriptorPool* p_descriptorPool, Device* p_device);
	std::vector<VkDescriptorPoolSize> DescriptorPool_buildDescriptorPoolSizeFromDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout);
}