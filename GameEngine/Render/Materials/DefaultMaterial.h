
#pragma once

#include <string.h>
#include "vulkan/vulkan.h"

#include "Render/Descriptor/DescriptorPool.h"
#include "Render/GraphcisPipeline/GraphicsPipeline.h"
#include "Render/Shader/Shader.h"
#include "Render/Shader/VertexInput.h"
#include "Render/Shader/DescriptorSetLayout.h"
#include "Render/Memory/VulkanBuffer.h"
#include "Render/Texture/Texture.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct SwapChain;
	struct CameraBufferSetupStep;
	struct PreRenderDeferedCommandBufferStep;
}

namespace _GameEngine::_Render
{
	struct DefaultMaterial
	{
		Shader VertexShader;
		Shader FragmentShader;
		GraphicsPipeline GraphicsPipeline;
		VertexInput VertexInput;

		// It contains one uniform buffer (the model projection buffer)
		DescriptorSetLayout DescriptorSetLayout;
		VkPipelineLayout PipelineLayout;
		DescriptorPool DescriptorPool;
	};

	struct DefaultMaterialAllocInfo
	{
		Device* Device;
		SwapChain* SwapChain;
		CameraBufferSetupStep* CameraBufferSetupStep;
	};
	
	void DefaultMaterial_alloc(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo);
	void DefaultMaterial_free(DefaultMaterial* p_defaultMaterial, Device* p_device);
	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo);

	struct DefaultMaterialInstance
	{
		DefaultMaterial* _DefaultMaterial;

		VulkanBuffer ModelProjectionBuffer;
		Texture Texture;

		VkDescriptorSet MaterialDescriptorSet;
	};

	struct DefaultMaterialInstanceAllocInfo
	{
		std::string TexturePath;
		VkSampler TextureSampler;
		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
	};

	struct ModelProjection
	{
		glm::mat4x4 Model;
	};

	void DefaultMaterialInstance_alloc(DefaultMaterialInstance* p_defaultMaterialInstance, DefaultMaterialInstanceAllocInfo* l_defaultMaterialInstanceAllocInfo,
				DefaultMaterial* p_defaultMaterial, Device* p_device);
	void DefaultMaterialInstance_free(DefaultMaterialInstance* p_defaultMaterialInstance, Device* p_device);

	void DefaultMaterialInsance_pushModelProjectionToGPU(DefaultMaterialInstance* p_defaultMaterialInstance, ModelProjection* p_modelProjection, Device* p_device);
};