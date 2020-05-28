
#pragma once

#include "vulkan/vulkan.h"

#include "Render/Descriptor/DescriptorPool.h"
#include "Render/GraphcisPipeline/GraphicsPipeline.h"
#include "Render/Shader/Shader.h"
#include "Render/Shader/VertexInput.h"
#include "Render/Shader/DescriptorSetLayout.h"
#include "Render/Memory/VulkanBuffer.h"

namespace _GameEngine::_Render
{
	struct Render;
}

namespace _GameEngine::_Render
{
	struct ModelProjection
	{
		glm::mat4x4 Model;
	};

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

	void DefaultMaterial_alloc(DefaultMaterial* p_defaultMaterial, Render* p_render);
	void DefaultMaterial_free(DefaultMaterial* p_defaultMaterial, Render* p_render);
	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterial* p_defaultMaterial, Render* p_render);

	struct DefaultMaterialInstance
	{
		DefaultMaterial* _DefaultMaterial;

		VulkanBuffer ModelProjectionBuffer;
		VkDescriptorSet ModelProjectionDescriptorSet;
	};

	void DefaultMaterialInstance_alloc(DefaultMaterialInstance* p_defaultMaterialInstance, DefaultMaterial* p_defaultMaterial, Render* p_render);
	void DefaultMaterialInstance_free(DefaultMaterialInstance* p_defaultMaterialInstance, Render* p_render);

	void DefaultMaterialInsance_pushModelProjectionToGPU(DefaultMaterialInstance* p_defaultMaterialInstance, ModelProjection* p_modelProjection, Render* p_render);
};