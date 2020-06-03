#pragma once

#include "vulkan/vulkan.h"
#include "glm/glm.hpp"

#include "Render/VulkanObjects/Memory/VulkanBuffer.h"
#include "Render/VulkanObjects/Descriptor/DescriptorPool.h"
#include "Render/Shader/DescriptorSetLayout.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct CameraProjection
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};

	struct CameraBufferSetupStep
	{
		CameraProjection CameraProjection;
		VulkanBuffer CameraProjectionBuffer;

		DescriptorPool DescriptorPool;
		VkPipelineLayout PipelineLayout;
		DescriptorSetLayout DescriptorSetLayout;
		VkDescriptorSet MaterialDescriptorSet;
	};

	void CameraBufferSetupStep_init(CameraBufferSetupStep* p_cameraDrawStep, Device* p_device);
	void CameraBufferSetupStep_free(CameraBufferSetupStep* p_cameraDrawStep, Device* p_device);
	void CameraBufferSetupStep_pushCameraPorjectionValueToGPU(CameraBufferSetupStep* p_cameraDrawStep, Device* p_device);
	void CameraBufferSetupStep_buildCommandBuffer(CameraBufferSetupStep* p_cameraDrawStep, VkCommandBuffer p_commandBuffer);
}