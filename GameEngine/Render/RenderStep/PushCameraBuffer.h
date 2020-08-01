#pragma once

#include "vulkan/vulkan.h"

#include "v2/Matrix/Matrix.hpp"

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
		_MathV2::Matrix4x4<float> View;
		_MathV2::Matrix4x4<float> Projection;
	};

	struct PushCameraBuffer
	{
		CameraProjection CameraProjection;
		VulkanBuffer CameraProjectionBuffer;

		DescriptorPool DescriptorPool;
		VkPipelineLayout PipelineLayout;
		DescriptorSetLayout DescriptorSetLayout;
		VkDescriptorSet MaterialDescriptorSet;
	};

	void PushCameraBuffer_init(PushCameraBuffer* p_pushCameraBuffer, Device* p_device);
	void PushCameraBuffer_free(PushCameraBuffer* p_pushCameraBuffer, Device* p_device);
	void PushCameraBuffer_pushToGPU(PushCameraBuffer* p_pushCameraBuffer, Device* p_device);
	void PushCameraBuffer_buildCommandBuffer(PushCameraBuffer* p_pushCameraBuffer, VkCommandBuffer p_commandBuffer);
}