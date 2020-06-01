#pragma once

#include <cstdint>
#include "vulkan/vulkan.h"

#include "Render/Memory/VulkanBuffer.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct Texture;
}

namespace _GameEngine::_Render
{
	struct UniformBufferParameter
	{
		uint32_t Binding;
		VkShaderStageFlags StageFlag;
		size_t BufferSize;
	};

	VkDescriptorSetLayoutBinding UniformBufferParameter_buildLayoutBinding(UniformBufferParameter* p_uniformBufferParameter);
	VulkanBuffer UniformBufferParameter_allocVulkanBuffer(UniformBufferParameter* p_uniformBufferParameter, Device* p_device);
	VkWriteDescriptorSet UniformBufferParameter_buildWriteDescriptorSet(UniformBufferParameter* p_uniformBufferParameter, VulkanBuffer* p_buffer, VkDescriptorBufferInfo* p_descriptorBufferInfo, VkDescriptorSet p_descriptorSet);
	
	struct ImageSampleParameter
	{
		uint32_t Binding;
		VkShaderStageFlags StageFlag;
		VkSampler TextureSampler;
	};

	VkDescriptorSetLayoutBinding ImageSampleParameter_buildLayoutBinding(ImageSampleParameter* p_imageSampleParameter);
	VkWriteDescriptorSet ImageSampleParameter_buildWriteDescriptorSet(ImageSampleParameter* p_imageSampleParameter, Texture* p_texture, VkDescriptorImageInfo* p_descriptorImageInfo, VkDescriptorSet p_descriptorSet);
};