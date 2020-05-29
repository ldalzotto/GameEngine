#pragma once

#include <vector>

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
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
	void DescriptorPool_freeUnique(DescriptorPool* p_descriptorPool, Device* p_device);
}