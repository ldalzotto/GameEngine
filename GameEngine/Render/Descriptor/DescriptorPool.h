#pragma once

#include "Render/Hardware/Device/Device.h"
#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct DescriptorPool
	{
		VkDescriptorPool DescriptorPool;
	};

	struct DescriptorPoolBuildInfo
	{
		uint32_t MaxDescriptorCount;

		// The set number is the number specified in the shader example : (layout(set = 1, binding = 0) uniform ModelProjectionBufferObject)
		uint32_t MaxSet;
		VkDescriptorPoolCreateFlags DescriptionPoolCreateFlags;
	};

	void DescriptorPool_buildUnique(DescriptorPool* p_descriptorPool, Device* p_device, DescriptorPoolBuildInfo* l_descriptoPoolBuildInfo);
	void DescriptorPool_freeUnique(DescriptorPool* p_descriptorPool, Device* p_device);
}