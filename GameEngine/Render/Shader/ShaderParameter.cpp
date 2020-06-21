#include "ShaderParameter.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include "Texture/Texture.h"

namespace _GameEngine::_Render
{
	void ShaderParameter_alloc(ShaderParameter* p_shaderParameter, ShaderParameterType p_shaderParameterType, ShaderParameterKey& p_keyName)
	{
		p_shaderParameter->Type = p_shaderParameterType;
		p_shaderParameter->KeyName = p_keyName;

		switch (p_shaderParameter->Type)
		{
		case ShaderParameterType::UNIFORM_BUFFER:
			p_shaderParameter->Parameter = calloc(1, sizeof(UniformBufferParameter));
			break;
		case ShaderParameterType::IMAGE_SAMPLER:
			p_shaderParameter->Parameter = calloc(1, sizeof(ImageSampleParameter));
			break;
		}
	};

	void ShaderParameter_free(ShaderParameter* p_shaderParameter)
	{
		free(p_shaderParameter->Parameter);
		p_shaderParameter->Parameter = nullptr;
	};

	VkDescriptorSetLayoutBinding UniformBufferParameter_buildLayoutBinding(UniformBufferParameter* p_uniformBufferParameter)
	{
		VkDescriptorSetLayoutBinding l_layoutBinding{};
		l_layoutBinding.binding = p_uniformBufferParameter->Binding;
		l_layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_layoutBinding.descriptorCount = 1;
		l_layoutBinding.stageFlags = p_uniformBufferParameter->StageFlag;
		l_layoutBinding.pImmutableSamplers = nullptr;
		return l_layoutBinding;
	};

	VulkanBuffer UniformBufferParameter_allocVulkanBuffer(UniformBufferParameter* p_uniformBufferParameter, Device* p_device)
	{
		VulkanBuffer l_buffer{};
		_Render::BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.Size = p_uniformBufferParameter->BufferSize;
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		_Render::VulkanBuffer_alloc(&l_buffer, &l_bufferAllocInfo, p_device);
		if (p_uniformBufferParameter->DefaultValue)
		{
			_Render::VulkanBuffer_pushToGPU(&l_buffer, p_device, p_uniformBufferParameter->DefaultValue, l_buffer.BufferAllocInfo.Size);
		}
		return std::move(l_buffer);
	};

	VkWriteDescriptorSet UniformBufferParameter_buildWriteDescriptorSet(UniformBufferParameter* p_uniformBufferParameter, VulkanBuffer* p_buffer,
		VkDescriptorBufferInfo* p_descriptorBufferInfo, VkDescriptorSet p_descriptorSet)
	{
		p_descriptorBufferInfo->buffer = p_buffer->Buffer;
		p_descriptorBufferInfo->offset = 0;
		p_descriptorBufferInfo->range = p_buffer->BufferAllocInfo.Size;

		VkWriteDescriptorSet l_descriptorUniforBufferWrite{};
		l_descriptorUniforBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_descriptorUniforBufferWrite.dstSet = p_descriptorSet;
		l_descriptorUniforBufferWrite.dstBinding = p_uniformBufferParameter->Binding;
		l_descriptorUniforBufferWrite.dstArrayElement = 0;
		l_descriptorUniforBufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorUniforBufferWrite.descriptorCount = 1;
		l_descriptorUniforBufferWrite.pBufferInfo = p_descriptorBufferInfo;
		l_descriptorUniforBufferWrite.pImageInfo = nullptr;
		l_descriptorUniforBufferWrite.pTexelBufferView = nullptr;
		return std::move(l_descriptorUniforBufferWrite);
	};


	VkDescriptorSetLayoutBinding ImageSampleParameter_buildLayoutBinding(ImageSampleParameter* p_imageSampleParameter)
	{
		VkDescriptorSetLayoutBinding l_layoutBinding{};
		l_layoutBinding.binding = p_imageSampleParameter->Binding;
		l_layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		l_layoutBinding.descriptorCount = 1;
		l_layoutBinding.stageFlags = p_imageSampleParameter->StageFlag;
		l_layoutBinding.pImmutableSamplers = nullptr;
		return l_layoutBinding;
	};

	VkWriteDescriptorSet ImageSampleParameter_buildWriteDescriptorSet(ImageSampleParameter* p_imageSampleParameter, Texture* p_texture, VkDescriptorImageInfo* p_descriptorImageInfo, VkDescriptorSet p_descriptorSet)
	{
		p_descriptorImageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		p_descriptorImageInfo->imageView = p_texture->ImageView.ImageView;
		p_descriptorImageInfo->sampler = p_imageSampleParameter->TextureSampler;

		VkWriteDescriptorSet l_imageDescriptorSet{};
		l_imageDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_imageDescriptorSet.dstSet = p_descriptorSet;
		l_imageDescriptorSet.dstBinding = p_imageSampleParameter->Binding;
		l_imageDescriptorSet.dstArrayElement = 0;
		l_imageDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		l_imageDescriptorSet.descriptorCount = 1;
		l_imageDescriptorSet.pImageInfo = p_descriptorImageInfo;
		return std::move(l_imageDescriptorSet);
	};
};