#include "DefaultMaterialV2.h"

#include <array>
#include <stdexcept>

#include "Log/Log.h"

#include "RenderInterface.h"
#include "VulkanObjects/Hardware/Device/Device.h"
#include "LoopStep/CameraBufferSetupStep.h"
#include "Mesh/Mesh.h"
#include "Texture/TextureSamplers.h"


namespace _GameEngine::_Render
{
	const uint32_t DEFAULTMATERIAL_MODEL_LAYOUT_BINDING = 0;
	const uint32_t DEFAULTMATERIAL_TEXTURE_LAYOUT_BINDING = 1;

	void setupExternalResources(DefaultMaterialV2_ExternalResources* p_externalResources);

	void createDescriptorSetLayout(DefaultMaterialV2_LocalInputParameters* p_localInputParameters, RenderInterface* p_renderInterface);
	void freeDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout, RenderInterface* p_renderInterface);

	void createDescriptorPool(DescriptorPool* p_descriptorPool, DescriptorSetLayout* p_descriptorSetLayout, RenderInterface* p_renderInterface);
	void freeDescriptorPool(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface);

	void createPipelineLayout(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface);
	void freePipelineLayout(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface);

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface);

	void DefaultMaterial_alloc(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		p_defaultMaterial->LocalInputParameters.ModelMatrix.Binding = DEFAULTMATERIAL_MODEL_LAYOUT_BINDING;
		p_defaultMaterial->LocalInputParameters.ModelMatrix.BufferSize = sizeof(ModelProjection);
		p_defaultMaterial->LocalInputParameters.ModelMatrix.StageFlag = VK_SHADER_STAGE_VERTEX_BIT;

		p_defaultMaterial->LocalInputParameters.BaseTexture.Binding = DEFAULTMATERIAL_TEXTURE_LAYOUT_BINDING;
		p_defaultMaterial->LocalInputParameters.BaseTexture.TextureSampler = p_renderInterface->TextureSamplers->DefaultSampler;
		p_defaultMaterial->LocalInputParameters.BaseTexture.StageFlag = VK_SHADER_STAGE_FRAGMENT_BIT;

		p_defaultMaterial->InternalResources.DepthBufferTexture = *p_renderInterface->DepthTexture;

		{
			setupExternalResources(&p_defaultMaterial->ExternalResources);
		}

		{
			auto l_localInputParameters = &p_defaultMaterial->LocalInputParameters;
			VertexInput_buildInput(&l_localInputParameters->VertexInput);
			createDescriptorSetLayout(l_localInputParameters, p_renderInterface);
			createDescriptorPool(&l_localInputParameters->DescriptorPool, &l_localInputParameters->DescriptorSetLayout, p_renderInterface);
		}

		{
			auto l_finalDrawObjects = &p_defaultMaterial->FinalDrawObjects;
			createPipelineLayout(p_defaultMaterial, p_renderInterface);
			GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_renderInterface);
			GraphicsPipeline_alloc(&l_finalDrawObjects->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
		}
	}

	void DefaultMaterial_free(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		GraphicsPipeline_free(&p_defaultMaterial->FinalDrawObjects.GraphicsPipeline);
		freePipelineLayout(p_defaultMaterial, p_renderInterface);
		freeDescriptorPool(p_defaultMaterial, p_renderInterface);
		freeDescriptorSetLayout(&p_defaultMaterial->LocalInputParameters.DescriptorSetLayout, p_renderInterface);
	};

	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		p_defaultMaterial->InternalResources.DepthBufferTexture = *p_renderInterface->DepthTexture;

		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_renderInterface);
		GraphicsPipeline_reallocatePipeline(&p_defaultMaterial->FinalDrawObjects.GraphicsPipeline, &l_graphicsPipelineAllocInfo);
	};

	void setupExternalResources(DefaultMaterialV2_ExternalResources* p_externalResources)
	{
		p_externalResources->VertexShader.ShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialVertex.spv";
		p_externalResources->VertexShader.ShaderType = ShaderType::VERTEX;

		p_externalResources->FragmentShader.ShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/TutorialFragment.spv";
		p_externalResources->FragmentShader.ShaderType = ShaderType::FRAGMENT;
	};

	void createDescriptorSetLayout(DefaultMaterialV2_LocalInputParameters* p_localInputParameters, RenderInterface* p_renderInterface)
	{
		DescriptorSetLayoutAllocInfo l_descriptorSetLayoutAllocInfo{};

		std::vector<VkDescriptorSetLayoutBinding> l_defaultMaterialDescriptorSetLayoutBindings
		{
			UniformBufferParameter_buildLayoutBinding(&p_localInputParameters->ModelMatrix),
			ImageSampleParameter_buildLayoutBinding(&p_localInputParameters->BaseTexture)
		};

		l_descriptorSetLayoutAllocInfo.LayoutBindings = &l_defaultMaterialDescriptorSetLayoutBindings;

		DescriptorSetLayout_alloc(&p_localInputParameters->DescriptorSetLayout, p_renderInterface->Device, &l_descriptorSetLayoutAllocInfo);
	};

	void freeDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout, RenderInterface* p_renderInterface)
	{
		DescriptorSetLayout_free(p_descriptorSetLayout, p_renderInterface->Device);
	};

	void createDescriptorPool(DescriptorPool* p_descritptorPool, DescriptorSetLayout* p_descriptorSetLayout, RenderInterface* p_renderInterface)
	{
		std::vector<VkDescriptorPoolSize> l_descriptorPoolSizes = DescriptorPool_buildDescriptorPoolSizeFromDescriptorSetLayout(p_descriptorSetLayout);

		DescriptorPoolAllocInfo l_descriptorPoolAllocInfo{};
		l_descriptorPoolAllocInfo.DescriptionPoolCreateFlags = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		l_descriptorPoolAllocInfo.SourceDescriptorPoolSize = &l_descriptorPoolSizes;
		l_descriptorPoolAllocInfo.MaxSet = 100000;
		DescriptorPool_alloc(p_descritptorPool, p_renderInterface->Device, &l_descriptorPoolAllocInfo);
	};

	void freeDescriptorPool(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		DescriptorPool_free(&p_defaultMaterial->LocalInputParameters.DescriptorPool, p_renderInterface->Device);
	};

	void createPipelineLayout(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		std::array<VkDescriptorSetLayout, 2> l_descriptorSetLayouts = {
					p_renderInterface->CameraBufferSetupStep->DescriptorSetLayout.DescriptorSetLayout,
					p_defaultMaterial->LocalInputParameters.DescriptorSetLayout.DescriptorSetLayout
		};

		VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
		l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		l_pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(l_descriptorSetLayouts.size());
		l_pipelineLayoutCreateInfo.pSetLayouts = l_descriptorSetLayouts.data();
		l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(p_renderInterface->Device->LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &p_defaultMaterial->FinalDrawObjects.PipelineLayout)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create pipeline layout!"));
		}
	};

	void freePipelineLayout(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		vkDestroyPipelineLayout(p_renderInterface->Device->LogicalDevice.LogicalDevice, p_defaultMaterial->FinalDrawObjects.PipelineLayout, nullptr);
	};

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterialV2* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo{};
		l_graphicsPipelineAllocInfo.VertexShader = &p_defaultMaterial->ExternalResources.VertexShader;
		l_graphicsPipelineAllocInfo.FragmentShader = &p_defaultMaterial->ExternalResources.FragmentShader;
		l_graphicsPipelineAllocInfo.PipelineLayout = p_defaultMaterial->FinalDrawObjects.PipelineLayout;
		l_graphicsPipelineAllocInfo.VertexInput = &p_defaultMaterial->LocalInputParameters.VertexInput;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.Device = p_renderInterface->Device;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.SwapChain = p_renderInterface->SwapChain;
	
		if (p_defaultMaterial->InternalResources.DepthBufferTexture != nullptr)
		{
			l_graphicsPipelineAllocInfo.GraphicsPipeline_DepthTest.DepthTexture = p_defaultMaterial->InternalResources.DepthBufferTexture;
		}

		return l_graphicsPipelineAllocInfo;
	};

}