
#pragma once

#include <string.h>
#include "vulkan/vulkan.h"

#include "VulkanObjects/Descriptor/DescriptorPool.h"
#include "VulkanObjects/GraphcisPipeline/GraphicsPipeline.h"
#include "Shader/Shader.h"
#include "Shader/VertexInput.h"
#include "Shader/DescriptorSetLayout.h"
#include "Shader/ShaderParameter.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct DefaultMaterialV2_ExternalResources
	{
		Shader VertexShader;
		Shader FragmentShader;
	};

	struct DefaultMaterialV2_InternalResources
	{
		Texture* DepthBufferTexture;
	};

	struct ModelProjection
	{
		glm::mat4x4 Model;
	};

	struct DefaultMaterialV2_LocalInputParameters
	{
		VertexInput VertexInput;

		std::vector<ShaderParameter> ShaderParameters;

		DescriptorSetLayout DescriptorSetLayout;
		DescriptorPool DescriptorPool;
	};

	struct DefaultMaterialV2
	{
		struct FinalDrawObjects
		{
			VkPipelineLayout PipelineLayout;
			GraphicsPipeline GraphicsPipeline;
		};

		DefaultMaterialV2_ExternalResources ExternalResources;
		DefaultMaterialV2_InternalResources InternalResources;
		DefaultMaterialV2_LocalInputParameters LocalInputParameters;
		FinalDrawObjects FinalDrawObjects;
	};

	struct MaterialAllocInfo
	{
		std::string VertexShader;
		std::string FragmentShader;
		std::vector<ShaderParameter> ShaderParameters;
	};

	void DefaultMaterial_alloc(DefaultMaterialV2* p_defaultMaterialV2, RenderInterface* p_renderInterface, MaterialAllocInfo* p_materialAllocInfo);
	void DefaultMaterial_free(DefaultMaterialV2* p_defaultMaterialV2, RenderInterface* p_renderInterface);
	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterialV2* p_defaultMaterialV2, RenderInterface* p_renderInterface);
};