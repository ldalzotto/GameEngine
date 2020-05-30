#include "DefaultMaterial.h"

#include <array>
#include <stdexcept>

#include "Log/Log.h"
#include "Render/LoopStep/CameraBufferSetupStep.h"
#include "Render/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	const uint32_t DEFAULTMATERIAL_MODEL_LAYOUT_BINDING = 0;
	const uint32_t DEFAULTMATERIAL_TEXTURE_LAYOUT_BINDING = 1;

	void createDescriptorPool(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo);
	void freeDescriptorPool(DefaultMaterial* p_defaultMaterial, Device* p_device);

	void createPipelineLayout(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo);
	void clearPipelineLayout(DefaultMaterial* p_defaultMaterial, Device* p_device);

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo);

	void DefaultMaterial_alloc(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo)
	{
		p_defaultMaterial->VertexShader.ShaderPath = "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialVertex.spv";
		p_defaultMaterial->VertexShader.ShaderType = ShaderType::VERTEX;

		p_defaultMaterial->FragmentShader.ShaderPath = "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialFragment.spv";
		p_defaultMaterial->FragmentShader.ShaderType = ShaderType::FRAGMENT;

		VertexInput_buildInput(&p_defaultMaterial->VertexInput);

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

		DescriptorSetLayout_alloc(&p_defaultMaterial->DescriptorSetLayout, p_defaultMaterialAllocInfo->Device, &l_descriptorSetLayoutAllocInfo);

		createDescriptorPool(p_defaultMaterial, p_defaultMaterialAllocInfo);
		createPipelineLayout(p_defaultMaterial, p_defaultMaterialAllocInfo);

		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_defaultMaterialAllocInfo);
		GraphicsPipeline_alloc(&p_defaultMaterial->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
	}

	void DefaultMaterial_free(DefaultMaterial* p_defaultMaterial, Device* p_device)
	{
		GraphicsPipeline_free(&p_defaultMaterial->GraphicsPipeline);
		clearPipelineLayout(p_defaultMaterial, p_device);
		freeDescriptorPool(p_defaultMaterial, p_device);
		DescriptorSetLayout_free(&p_defaultMaterial->DescriptorSetLayout, p_device);
	};

	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo)
	{
		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_defaultMaterialAllocInfo);
		GraphicsPipeline_reallocatePipeline(&p_defaultMaterial->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
	};

	void createDescriptorPool(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo)
	{
		std::vector<VkDescriptorPoolSize> l_descriptorPoolSizes(2);
		memset(l_descriptorPoolSizes.data(), 0, sizeof(VkDescriptorPoolSize) * l_descriptorPoolSizes.size());

		VkDescriptorPoolSize* l_modelBufferDescriptorPoolSize = &l_descriptorPoolSizes.at(0);
		l_modelBufferDescriptorPoolSize->descriptorCount = 1;
		l_modelBufferDescriptorPoolSize->type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		VkDescriptorPoolSize* l_textureSamplerDescriptorPoolSize = &l_descriptorPoolSizes.at(1);
		l_textureSamplerDescriptorPoolSize->descriptorCount = 1;
		l_textureSamplerDescriptorPoolSize->type = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		DescriptorPoolBuildInfo l_descriptorPoolBuildInfo{};
		l_descriptorPoolBuildInfo.DescriptionPoolCreateFlags = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		l_descriptorPoolBuildInfo.DescriptorPoolSizes = &l_descriptorPoolSizes;
		l_descriptorPoolBuildInfo.MaxSet = 20;
		DescriptorPool_buildUnique(&p_defaultMaterial->DescriptorPool, p_defaultMaterialAllocInfo->Device, &l_descriptorPoolBuildInfo);
	};

	void freeDescriptorPool(DefaultMaterial* p_defaultMaterial, Device* p_device)
	{
		DescriptorPool_freeUnique(&p_defaultMaterial->DescriptorPool, p_device);
	};

	void createPipelineLayout(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo)
	{
		std::array<VkDescriptorSetLayout, 2> l_descriptorSetLayouts = {
					p_defaultMaterialAllocInfo->CameraBufferSetupStep->DescriptorSetLayout.DescriptorSetLayout,
					p_defaultMaterial->DescriptorSetLayout.DescriptorSetLayout
		};

		VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
		l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		l_pipelineLayoutCreateInfo.setLayoutCount = l_descriptorSetLayouts.size();
		l_pipelineLayoutCreateInfo.pSetLayouts = l_descriptorSetLayouts.data();
		l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(p_defaultMaterialAllocInfo->Device->LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &p_defaultMaterial->PipelineLayout)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create pipeline layout!"));
		}
	};

	void clearPipelineLayout(DefaultMaterial* p_defaultMaterial, Device* p_device)
	{
		vkDestroyPipelineLayout(p_device->LogicalDevice.LogicalDevice, p_defaultMaterial->PipelineLayout, nullptr);
	};


	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterial* p_defaultMaterial, DefaultMaterialAllocInfo* p_defaultMaterialAllocInfo)
	{
		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo{};
		l_graphicsPipelineAllocInfo.VertexShader = &p_defaultMaterial->VertexShader;
		l_graphicsPipelineAllocInfo.FragmentShader = &p_defaultMaterial->FragmentShader;
		l_graphicsPipelineAllocInfo.PipelineLayout = p_defaultMaterial->PipelineLayout;
		l_graphicsPipelineAllocInfo.VertexInput = &p_defaultMaterial->VertexInput;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.Device = p_defaultMaterialAllocInfo->Device;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.SwapChain = p_defaultMaterialAllocInfo->SwapChain;
		return l_graphicsPipelineAllocInfo;
	};





	void DefaultMaterialInstance_alloc(DefaultMaterialInstance* p_defaultMaterialInstance, DefaultMaterialInstanceAllocInfo* l_defaultMaterialInstanceAllocInfo, DefaultMaterial* p_defaultMaterial, Device* p_device)
	{
		p_defaultMaterialInstance->_DefaultMaterial = p_defaultMaterial;

		// Allocate Texture
		TextureLoadInfo l_textureLoadInfo{};
		l_textureLoadInfo.Device = p_device;
		l_textureLoadInfo.PreRenderDeferedCommandBufferStep = l_defaultMaterialInstanceAllocInfo->PreRenderDeferedCommandBufferStep;
		Texture_load(&p_defaultMaterialInstance->Texture, l_defaultMaterialInstanceAllocInfo->TexturePath, &l_textureLoadInfo);

		// Descriptor set
		_Render::BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.Size = sizeof(_Render::ModelProjection);
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		_Render::VulkanBuffer_alloc(&p_defaultMaterialInstance->ModelProjectionBuffer, &l_bufferAllocInfo, p_device);

		VkDescriptorSetAllocateInfo l_descriptorSetAllocateInfo{};
		l_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		l_descriptorSetAllocateInfo.descriptorPool = p_defaultMaterialInstance->_DefaultMaterial->DescriptorPool.DescriptorPool;
		l_descriptorSetAllocateInfo.descriptorSetCount = 1;
		l_descriptorSetAllocateInfo.pSetLayouts = &p_defaultMaterialInstance->_DefaultMaterial->DescriptorSetLayout.DescriptorSetLayout;
		if (vkAllocateDescriptorSets(p_device->LogicalDevice.LogicalDevice, &l_descriptorSetAllocateInfo, &p_defaultMaterialInstance->ModelProjectionDescriptorSet) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create description set."));
		};

		std::vector<VkWriteDescriptorSet> l_writeDescirptorSets(2);


		VkDescriptorBufferInfo l_descriptorUniformBufferInfo{};
		l_descriptorUniformBufferInfo.buffer = p_defaultMaterialInstance->ModelProjectionBuffer.Buffer;
		l_descriptorUniformBufferInfo.offset = 0;
		l_descriptorUniformBufferInfo.range = p_defaultMaterialInstance->ModelProjectionBuffer.BufferAllocInfo.Size;
		VkWriteDescriptorSet* l_descriptorUniforBufferWrite = &l_writeDescirptorSets.at(0);
		l_descriptorUniforBufferWrite->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_descriptorUniforBufferWrite->dstSet = p_defaultMaterialInstance->ModelProjectionDescriptorSet;
		l_descriptorUniforBufferWrite->dstBinding = DEFAULTMATERIAL_MODEL_LAYOUT_BINDING;
		l_descriptorUniforBufferWrite->dstArrayElement = 0;
		l_descriptorUniforBufferWrite->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorUniforBufferWrite->descriptorCount = 1;
		l_descriptorUniforBufferWrite->pBufferInfo = &l_descriptorUniformBufferInfo;
		l_descriptorUniforBufferWrite->pImageInfo = nullptr;
		l_descriptorUniforBufferWrite->pTexelBufferView = nullptr;

		VkDescriptorImageInfo l_descriptorImageInfo{};
		l_descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		l_descriptorImageInfo.imageView = p_defaultMaterialInstance->Texture.ImageView.ImageView;
		l_descriptorImageInfo.sampler = l_defaultMaterialInstanceAllocInfo->TextureSampler;
		VkWriteDescriptorSet* l_imageDescriptorSet = &l_writeDescirptorSets.at(1);
		l_imageDescriptorSet->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_imageDescriptorSet->dstSet = p_defaultMaterialInstance->ModelProjectionDescriptorSet;
		l_imageDescriptorSet->dstBinding = DEFAULTMATERIAL_TEXTURE_LAYOUT_BINDING;
		l_imageDescriptorSet->dstArrayElement = 0;
		l_imageDescriptorSet->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		l_imageDescriptorSet->descriptorCount = 1;
		l_imageDescriptorSet->pImageInfo = &l_descriptorImageInfo;

		vkUpdateDescriptorSets(p_device->LogicalDevice.LogicalDevice, l_writeDescirptorSets.size(), l_writeDescirptorSets.data(), 0, nullptr);
	};

	void DefaultMaterialInstance_free(DefaultMaterialInstance* p_defaultMaterialInstance, Device* p_device)
	{
		vkFreeDescriptorSets(p_device->LogicalDevice.LogicalDevice, p_defaultMaterialInstance->_DefaultMaterial->DescriptorPool.DescriptorPool, 1, &p_defaultMaterialInstance->ModelProjectionDescriptorSet);
		_Render::VulkanBuffer_free(&p_defaultMaterialInstance->ModelProjectionBuffer, p_device);
		Texture_free(&p_defaultMaterialInstance->Texture, p_device);
	};

	void DefaultMaterialInsance_pushModelProjectionToGPU(DefaultMaterialInstance* p_defaultMaterialInstance, ModelProjection* p_modelProjection, Device* p_device)
	{
		VulkanBuffer_pushToGPU(&p_defaultMaterialInstance->ModelProjectionBuffer, p_device, p_modelProjection, sizeof(ModelProjection));
	};
};