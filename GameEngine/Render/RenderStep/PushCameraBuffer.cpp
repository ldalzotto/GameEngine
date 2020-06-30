#include "PushCameraBuffer.h"

#include "VulkanObjects/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	const uint32_t CAMERA_BUFFER_LAYOUT_BINDING = 0;

	void PushCameraBuffer_init(PushCameraBuffer* p_pushCameraBuffer, Device* p_device)
	{
		std::vector<VkDescriptorPoolSize> l_descriptorPoolSize(1);
		VkDescriptorPoolSize* l_cameraBufferDescriptorPooSize = &l_descriptorPoolSize.at(0);
		l_cameraBufferDescriptorPooSize->type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_cameraBufferDescriptorPooSize->descriptorCount = 1;

		DescriptorPoolAllocInfo l_descriptorPoolAllocInfo{};
		l_descriptorPoolAllocInfo.SourceDescriptorPoolSize = &l_descriptorPoolSize;
		l_descriptorPoolAllocInfo.MaxSet = 1;
		DescriptorPool_alloc(&p_pushCameraBuffer->DescriptorPool, p_device, &l_descriptorPoolAllocInfo);


		std::vector<VkDescriptorSetLayoutBinding> l_descriptorSetLayoutBindings(1);
		VkDescriptorSetLayoutBinding l_cameraBufferLayoutBinding{};
		l_cameraBufferLayoutBinding.binding = CAMERA_BUFFER_LAYOUT_BINDING;
		l_cameraBufferLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_cameraBufferLayoutBinding.descriptorCount = 1;
		l_cameraBufferLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		l_cameraBufferLayoutBinding.pImmutableSamplers = nullptr;
		l_descriptorSetLayoutBindings.at(0) = l_cameraBufferLayoutBinding;

		DescriptorSetLayoutAllocInfo l_descriptorSetLayoutAllocInfo{};
		l_descriptorSetLayoutAllocInfo.LayoutBindings = &l_descriptorSetLayoutBindings;
		DescriptorSetLayout_alloc(&p_pushCameraBuffer->DescriptorSetLayout, p_device, &l_descriptorSetLayoutAllocInfo);

		VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
		l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		l_pipelineLayoutCreateInfo.setLayoutCount = 1;
		l_pipelineLayoutCreateInfo.pSetLayouts = &p_pushCameraBuffer->DescriptorSetLayout.DescriptorSetLayout;
		l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
		vkCreatePipelineLayout(p_device->LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &p_pushCameraBuffer->PipelineLayout);

		VkDescriptorSetAllocateInfo l_descriptorSetAllocateInfo{};
		l_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		l_descriptorSetAllocateInfo.descriptorPool = p_pushCameraBuffer->DescriptorPool.DescriptorPool;
		l_descriptorSetAllocateInfo.descriptorSetCount = 1;
		l_descriptorSetAllocateInfo.pSetLayouts = &p_pushCameraBuffer->DescriptorSetLayout.DescriptorSetLayout;
		vkAllocateDescriptorSets(p_device->LogicalDevice.LogicalDevice, &l_descriptorSetAllocateInfo, &p_pushCameraBuffer->MaterialDescriptorSet);

		BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.Size = sizeof(CameraProjection);
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		VulkanBuffer_alloc(&p_pushCameraBuffer->CameraProjectionBuffer, &l_bufferAllocInfo, p_device);

		VkDescriptorBufferInfo l_descriptorUniformBufferInfo{};
		l_descriptorUniformBufferInfo.buffer = p_pushCameraBuffer->CameraProjectionBuffer.Buffer;
		l_descriptorUniformBufferInfo.offset = 0;
		l_descriptorUniformBufferInfo.range = p_pushCameraBuffer->CameraProjectionBuffer.BufferAllocInfo.Size;

		VkWriteDescriptorSet l_descriptorUniforBufferWrite{};
		l_descriptorUniforBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_descriptorUniforBufferWrite.dstSet = p_pushCameraBuffer->MaterialDescriptorSet;
		l_descriptorUniforBufferWrite.dstBinding = CAMERA_BUFFER_LAYOUT_BINDING;
		l_descriptorUniforBufferWrite.dstArrayElement = 0;
		l_descriptorUniforBufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorUniforBufferWrite.descriptorCount = 1;
		l_descriptorUniforBufferWrite.pBufferInfo = &l_descriptorUniformBufferInfo;
		l_descriptorUniforBufferWrite.pImageInfo = nullptr;
		l_descriptorUniforBufferWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(p_device->LogicalDevice.LogicalDevice, 1, &l_descriptorUniforBufferWrite, 0, nullptr);
	};

	void PushCameraBuffer_free(PushCameraBuffer* p_pushCameraBuffer, Device* p_device)
	{
		VulkanBuffer_free(&p_pushCameraBuffer->CameraProjectionBuffer, p_device);
		vkDestroyPipelineLayout(p_device->LogicalDevice.LogicalDevice, p_pushCameraBuffer->PipelineLayout, nullptr);
		DescriptorSetLayout_free(&p_pushCameraBuffer->DescriptorSetLayout, p_device);
		DescriptorPool_free(&p_pushCameraBuffer->DescriptorPool, p_device);
	};

	void PushCameraBuffer_pushToGPU(PushCameraBuffer* p_pushCameraBuffer, Device* p_device)
	{
		_Render::VulkanBuffer_pushToGPU(&p_pushCameraBuffer->CameraProjectionBuffer, p_device, &p_pushCameraBuffer->CameraProjection, p_pushCameraBuffer->CameraProjectionBuffer.BufferAllocInfo.Size);
	};


	void PushCameraBuffer_buildCommandBuffer(PushCameraBuffer* p_pushCameraBuffer, VkCommandBuffer p_commandBuffer)
	{
		vkCmdBindDescriptorSets(p_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_pushCameraBuffer->PipelineLayout, 0, 1, &p_pushCameraBuffer->MaterialDescriptorSet, 0, nullptr);
	};
}