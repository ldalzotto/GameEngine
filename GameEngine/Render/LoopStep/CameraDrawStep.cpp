#include "CameraDrawStep.h"

namespace _GameEngine::_Render
{
	void CameraDrawStep_init(CameraDrawStep* p_cameraDrawStep, Device* p_device)
	{
		DescriptorPool_buildUnique(&p_cameraDrawStep->DescriptorPool, p_device);

		DescriptorSetLayoutAllocInfo l_descriptorSetLayoutAllocInfo{};
		l_descriptorSetLayoutAllocInfo.Binding = 0;
		DescriptorSetLayout_alloc(&p_cameraDrawStep->DescriptorSetLayout, p_device, &l_descriptorSetLayoutAllocInfo);

		VkDescriptorSetAllocateInfo l_descriptorSetAllocateInfo{};
		l_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		l_descriptorSetAllocateInfo.descriptorPool = p_cameraDrawStep->DescriptorPool.DescriptorPool;
		l_descriptorSetAllocateInfo.descriptorSetCount = 1;
		l_descriptorSetAllocateInfo.pSetLayouts = &p_cameraDrawStep->DescriptorSetLayout.DescriptorSetLayout;
		vkAllocateDescriptorSets(p_device->LogicalDevice.LogicalDevice, &l_descriptorSetAllocateInfo, &p_cameraDrawStep->DescriptorSet);

		BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.Size = sizeof(CameraProjection);
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		VulkanBuffer_alloc(&p_cameraDrawStep->CameraProjectionBuffer, &l_bufferAllocInfo, p_device);

		VkDescriptorBufferInfo l_descriptorUniformBufferInfo{};
		l_descriptorUniformBufferInfo.buffer = p_cameraDrawStep->CameraProjectionBuffer.Buffer;
		l_descriptorUniformBufferInfo.offset = 0;
		l_descriptorUniformBufferInfo.range = p_cameraDrawStep->CameraProjectionBuffer.BufferAllocInfo.Size;

		VkWriteDescriptorSet l_descriptorUniforBufferWrite{};
		l_descriptorUniforBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_descriptorUniforBufferWrite.dstSet = p_cameraDrawStep->DescriptorSet;
		l_descriptorUniforBufferWrite.dstBinding = 0;
		l_descriptorUniforBufferWrite.dstArrayElement = 0;
		l_descriptorUniforBufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorUniforBufferWrite.descriptorCount = 1;
		l_descriptorUniforBufferWrite.pBufferInfo = &l_descriptorUniformBufferInfo;
		l_descriptorUniforBufferWrite.pImageInfo = nullptr;
		l_descriptorUniforBufferWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(p_device->LogicalDevice.LogicalDevice, 1, &l_descriptorUniforBufferWrite, 0, nullptr);
	};

	void CameraDrawStep_free(CameraDrawStep* p_cameraDrawStep, Device* p_device)
	{
		VulkanBuffer_free(&p_cameraDrawStep->CameraProjectionBuffer, p_device);
		DescriptorSetLayout_free(&p_cameraDrawStep->DescriptorSetLayout, p_device);
		DescriptorPool_freeUnique(&p_cameraDrawStep->DescriptorPool, p_device);
	};

	void CameraDrawStep_pushCameraPorjectionValueToGPU(CameraDrawStep* p_cameraDrawStep, Device* p_device)
	{
		_Render::VulkanBuffer_pushToGPU(&p_cameraDrawStep->CameraProjectionBuffer, p_device, &p_cameraDrawStep->CameraProjection, p_cameraDrawStep->CameraProjectionBuffer.BufferAllocInfo.Size);
	};
}