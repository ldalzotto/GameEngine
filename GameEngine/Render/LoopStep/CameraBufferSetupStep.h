#pragma once

#include "vulkan/vulkan.h"

#include "Math/Matrix/Matrix.h"

#include "VulkanObjects/Memory/VulkanBuffer.h"
#include "VulkanObjects/Descriptor/DescriptorPool.h"
#include "Shader/DescriptorSetLayout.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{
	struct CameraProjection
	{
		_Math::Matrix4x4f View;
		_Math::Matrix4x4f Projection;
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