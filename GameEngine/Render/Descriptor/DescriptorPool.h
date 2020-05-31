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

	struct DescriptorPoolBuildInfo
	{
		std::vector<VkDescriptorPoolSize>* DescriptorPoolSizes;
		uint32_t MaxSet;
		VkDescriptorPoolCreateFlags DescriptionPoolCreateFlags;
	};

	void DescriptorPool_buildUnique(DescriptorPool* p_descriptorPool, Device* p_device, DescriptorPoolBuildInfo* l_descriptoPoolBuildInfo);

	struct DescriptorPoolAllocInfo
	{
		uint32_t MaxSet;
		VkDescriptorPoolCreateFlags DescriptionPoolCreateFlags;
		DescriptorSetLayout* DescriptorSetLayout;
	};

	void DescriptorPool_alloc(DescriptorPool* p_descriptorPool, Device* p_device, DescriptorPoolAllocInfo* p_descriptorPoolAllocInfo);
	void DescriptorPool_freeUnique(DescriptorPool* p_descriptorPool, Device* p_device);

}