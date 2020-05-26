
#include "vulkan/vulkan.h"
#include "Render/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	struct DescriptorSetLayout
	{
		VkDescriptorSetLayout DescriptorSetLayout;
	};

	void DescriptorSetLayout_alloc(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device);
	void DescriptorSetLayout_free(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device);
}