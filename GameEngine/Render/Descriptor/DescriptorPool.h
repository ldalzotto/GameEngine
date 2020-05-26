#pragma once

#include "Render/Hardware/Device/Device.h"
#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct DescriptorPool
	{
		VkDescriptorPool DescriptorPool;
	};

	void DescriptorPool_buildUnique(DescriptorPool* p_descriptorPool, Device* p_device);
	void DescriptorPool_freeUnique(DescriptorPool* p_descriptorPool, Device* p_device);
}