#include "DefaultMaterialV2.h"

#include <array>
#include <stdexcept>

#include "Log/Log.h"
#include "Render/Hardware/Device/Device.h"
#include "Render/LoopStep/CameraBufferSetupStep.h"

namespace _GameEngine::_Render
{
	const uint32_t DEFAULTMATERIAL_MODEL_LAYOUT_BINDING = 0;
	const uint32_t DEFAULTMATERIAL_TEXTURE_LAYOUT_BINDING = 1;

	void setupExternalResources(DefaultMaterialV2_ExternalResources* p_externalResources);

	void createDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo);
	void freeDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device);

	void createDescriptorPool(DescriptorPool* p_descriptorPool, DescriptorSetLayout* p_descriptorSetLayout, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo);
	void freeDescriptorPool(DefaultMaterialV2* p_defaultMaterial, Device* p_device);

	void createPipelineLayout(DefaultMaterialV2* p_defaultMaterial, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo);
	void freePipelineLayout(DefaultMaterialV2* p_defaultMaterial, Device* p_device);

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterialV2* p_defaultMaterial, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo);

	void DefaultMaterial_alloc(DefaultMaterialV2* p_defaultMaterial, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo)
	{
		setupExternalResources(&p_defaultMaterial->ExternalResources);

		auto l_localInputParameters = &p_defaultMaterial->LocalInputParameters;
		{
			VertexInput_buildInput(&l_localInputParameters->VertexInput);
			createDescriptorSetLayout(&l_localInputParameters->DescriptorSetLayout, p_defaultMaterialAllocInfo);
			createDescriptorPool(&l_localInputParameters->DescriptorPool, &l_localInputParameters->DescriptorSetLayout, p_defaultMaterialAllocInfo);
		}

		auto l_finalDrawObjects = &p_defaultMaterial->FinalDrawObjects;
		{
			createPipelineLayout(p_defaultMaterial, p_defaultMaterialAllocInfo);
			GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_defaultMaterialAllocInfo);
			GraphicsPipeline_alloc(&l_finalDrawObjects->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
		}
	}

	void DefaultMaterial_free(DefaultMaterialV2* p_defaultMaterial, Device* p_device)
	{
		GraphicsPipeline_free(&p_defaultMaterial->FinalDrawObjects.GraphicsPipeline);
		freePipelineLayout(p_defaultMaterial, p_device);
		freeDescriptorPool(p_defaultMaterial, p_device);
		freeDescriptorSetLayout(&p_defaultMaterial->LocalInputParameters.DescriptorSetLayout, p_device);
	};

	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterialV2* p_defaultMaterial, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo)
	{
		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_defaultMaterialAllocInfo);
		GraphicsPipeline_reallocatePipeline(&p_defaultMaterial->FinalDrawObjects.GraphicsPipeline, &l_graphicsPipelineAllocInfo);
	};

	void setupExternalResources(DefaultMaterialV2_ExternalResources* p_externalResources)
	{
		p_externalResources->VertexShader.ShaderPath = "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialVertex.spv";
		p_externalResources->VertexShader.ShaderType = ShaderType::VERTEX;

		p_externalResources->FragmentShader.ShaderPath = "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialFragment.spv";
		p_externalResources->FragmentShader.ShaderType = ShaderType::FRAGMENT;
	};

	void createDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo)
	{
		DescriptorSetLayoutAllocInfo l_descriptorSetLayoutAllocInfo{};

		std::vector<VkDescriptorSetLayoutBinding> l_defaultMaterialDescriptorSetLayoutBindings(2);

		VkDescriptorSetLayoutBinding l_modelLayoutBinding{};
		l_modelLayoutBinding.binding = DEFAULTMATERIAL_MODEL_LAYOUT_BINDING;
		l_modelLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_modelLayoutBinding.descriptorCount = 1;
		l_modelLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		l_modelLayoutBinding.pImmutableSamplers = nullptr;
		l_defaultMaterialDescriptorSetLayoutBindings.at(0) = l_modelLayoutBinding;

		VkDescriptorSetLayoutBinding l_textureSamplerBinding{};
		l_textureSamplerBinding.binding = DEFAULTMATERIAL_TEXTURE_LAYOUT_BINDING;
		l_textureSamplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		l_textureSamplerBinding.descriptorCount = 1;
		l_textureSamplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		l_textureSamplerBinding.pImmutableSamplers = nullptr;
		l_defaultMaterialDescriptorSetLayoutBindings.at(1) = l_textureSamplerBinding;


		l_descriptorSetLayoutAllocInfo.LayoutBindings = &l_defaultMaterialDescriptorSetLayoutBindings;

		DescriptorSetLayout_alloc(p_descriptorSetLayout, p_defaultMaterialAllocInfo->Device, &l_descriptorSetLayoutAllocInfo);
	};

	void freeDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout, Device* p_device)
	{
		DescriptorSetLayout_free(p_descriptorSetLayout, p_device);
	};

	void createDescriptorPool(DescriptorPool* p_descritptorPool, DescriptorSetLayout* p_descriptorSetLayout,  DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo)
	{
		std::vector<VkDescriptorPoolSize> l_descriptorPoolSizes = DescriptorPool_buildDescriptorPoolSizeFromDescriptorSetLayout(p_descriptorSetLayout);

		DescriptorPoolAllocInfo l_descriptorPoolAllocInfo{};
		l_descriptorPoolAllocInfo.DescriptionPoolCreateFlags = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		l_descriptorPoolAllocInfo.SourceDescriptorPoolSize = &l_descriptorPoolSizes;
		l_descriptorPoolAllocInfo.MaxSet = 20;
		DescriptorPool_alloc(p_descritptorPool, p_defaultMaterialAllocInfo->Device, &l_descriptorPoolAllocInfo);
	};

	void freeDescriptorPool(DefaultMaterialV2* p_defaultMaterial, Device* p_device)
	{
		DescriptorPool_free(&p_defaultMaterial->LocalInputParameters.DescriptorPool, p_device);
	};

	void createPipelineLayout(DefaultMaterialV2* p_defaultMaterial, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo)
	{
		std::array<VkDescriptorSetLayout, 2> l_descriptorSetLayouts = {
					p_defaultMaterialAllocInfo->CameraBufferSetupStep->DescriptorSetLayout.DescriptorSetLayout,
					p_defaultMaterial->LocalInputParameters.DescriptorSetLayout.DescriptorSetLayout
		};

		VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
		l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		l_pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(l_descriptorSetLayouts.size());
		l_pipelineLayoutCreateInfo.pSetLayouts = l_descriptorSetLayouts.data();
		l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(p_defaultMaterialAllocInfo->Device->LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &p_defaultMaterial->FinalDrawObjects.PipelineLayout)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create pipeline layout!"));
		}
	};

	void freePipelineLayout(DefaultMaterialV2* p_defaultMaterial, Device* p_device)
	{
		vkDestroyPipelineLayout(p_device->LogicalDevice.LogicalDevice, p_defaultMaterial->FinalDrawObjects.PipelineLayout, nullptr);
	};

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterialV2* p_defaultMaterial, DefaultMaterialV2AllocInfo* p_defaultMaterialAllocInfo)
	{
		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo{};
		l_graphicsPipelineAllocInfo.VertexShader = &p_defaultMaterial->ExternalResources.VertexShader;
		l_graphicsPipelineAllocInfo.FragmentShader = &p_defaultMaterial->ExternalResources.FragmentShader;
		l_graphicsPipelineAllocInfo.PipelineLayout = p_defaultMaterial->FinalDrawObjects.PipelineLayout;
		l_graphicsPipelineAllocInfo.VertexInput = &p_defaultMaterial->LocalInputParameters.VertexInput;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.Device = p_defaultMaterialAllocInfo->Device;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.SwapChain = p_defaultMaterialAllocInfo->SwapChain;
		return l_graphicsPipelineAllocInfo;
	};

}