#pragma once

#include <cstdint>
#include "vulkan/vulkan.h"

#include "VulkanObjects/Memory/VulkanBuffer.h"
#include "ShaderParameterKeys.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct Texture;
}

namespace _GameEngine::_Render
{
	enum class ShaderParameterType : uint8_t
	{
		UNIFORM_BUFFER,
		IMAGE_SAMPLER
	};

	struct ShaderParameter
	{
		ShaderParameterType Type;
		ShaderParameterKey KeyName;
		VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding;
		void* Parameter;
	};

	void ShaderParameter_alloc(ShaderParameter* p_shaderParameter, ShaderParameterType p_shaderParameterType, ShaderParameterKey& p_keyName);
	void ShaderParameter_free(ShaderParameter* p_shaderParameter);

	struct UniformBufferParameter
	{
		uint32_t Binding;
		VkShaderStageFlags StageFlag;
		size_t BufferSize;
		void* DefaultValue;
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