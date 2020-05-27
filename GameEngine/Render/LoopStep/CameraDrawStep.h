#pragma once

#include "vulkan/vulkan.h"
#include "glm/glm.hpp"

#include "Render/Memory/VulkanBuffer.h"
#include "Render/Descriptor/DescriptorPool.h"
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

	struct CameraDrawStep
	{
		CameraProjection CameraProjection;
		VulkanBuffer CameraProjectionBuffer;

		DescriptorPool DescriptorPool;
		DescriptorSetLayout DescriptorSetLayout;
		VkDescriptorSet DescriptorSet;
	};

	void CameraDrawStep_init(CameraDrawStep* p_cameraDrawStep, Device* p_device);
	void CameraDrawStep_free(CameraDrawStep* p_cameraDrawStep, Device* p_device);
	void CameraDrawStep_pushCameraPorjectionValueToGPU(CameraDrawStep* p_cameraDrawStep, Device* p_device);
}