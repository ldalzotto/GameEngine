#include "DefaultMaterial.h"

#include <array>
#include <stdexcept>

#include "Log/Log.h"
#include "Render/Render.h"

namespace _GameEngine::_Render
{

	void createDescriptorPool(DefaultMaterial* p_defaultMaterial, Render* p_render);
	void freeDescriptorPool(DefaultMaterial* p_defaultMaterial, Render* p_render);

	void createPipelineLayout(DefaultMaterial* p_defaultMaterial, Render* p_render);
	void clearPipelineLayout(DefaultMaterial* p_defaultMaterial, Render* p_render);

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterial* p_defaultMaterial, Render* p_render);

	void DefaultMaterial_alloc(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		p_defaultMaterial->VertexShader.ShaderPath = "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialVertex.spv";
		p_defaultMaterial->VertexShader.ShaderType = ShaderType::VERTEX;

		p_defaultMaterial->FragmentShader.ShaderPath = "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialFragment.spv";
		p_defaultMaterial->FragmentShader.ShaderType = ShaderType::FRAGMENT;

		VertexInput_buildInput(&p_defaultMaterial->VertexInput);

		DescriptorSetLayoutAllocInfo l_descriptorSetLayoutAllocInfo{};
		l_descriptorSetLayoutAllocInfo.Binding = 0;
		DescriptorSetLayout_alloc(&p_defaultMaterial->DescriptorSetLayout, &p_render->Device, &l_descriptorSetLayoutAllocInfo);

		createDescriptorPool(p_defaultMaterial, p_render);
		createPipelineLayout(p_defaultMaterial, p_render);

		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_render);
		GraphicsPipeline_alloc(&p_defaultMaterial->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
	}

	void DefaultMaterial_free(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		GraphicsPipeline_free(&p_defaultMaterial->GraphicsPipeline);
		clearPipelineLayout(p_defaultMaterial, p_render);
		freeDescriptorPool(p_defaultMaterial, p_render);
		DescriptorSetLayout_free(&p_defaultMaterial->DescriptorSetLayout, &p_render->Device);
	};

	void DefaultMaterial_reAllocGraphicsPipeline(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_render);
		GraphicsPipeline_reallocatePipeline(&p_defaultMaterial->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
	};

	void createDescriptorPool(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		DescriptorPoolBuildInfo l_descriptorPoolBuildInfo{};
		// To allow realeasing individual descriptor sets from material instances
		l_descriptorPoolBuildInfo.DescriptionPoolCreateFlags = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		l_descriptorPoolBuildInfo.MaxDescriptorCount = 2;
		l_descriptorPoolBuildInfo.MaxSet = 2;
		DescriptorPool_buildUnique(&p_defaultMaterial->DescriptorPool, &p_render->Device, &l_descriptorPoolBuildInfo);
	};

	void freeDescriptorPool(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		DescriptorPool_freeUnique(&p_defaultMaterial->DescriptorPool, &p_render->Device);
	};

	void createPipelineLayout(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		std::array<VkDescriptorSetLayout, 2> l_descriptorSetLayouts = {
					p_render->CameraBufferSetupStep.DescriptorSetLayout.DescriptorSetLayout,
					p_defaultMaterial->DescriptorSetLayout.DescriptorSetLayout
		};

		VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
		l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		l_pipelineLayoutCreateInfo.setLayoutCount = l_descriptorSetLayouts.size();
		l_pipelineLayoutCreateInfo.pSetLayouts = l_descriptorSetLayouts.data();
		l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(p_render->Device.LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &p_defaultMaterial->PipelineLayout)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create pipeline layout!"));
		}
	};

	void clearPipelineLayout(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		vkDestroyPipelineLayout(p_render->Device.LogicalDevice.LogicalDevice, p_defaultMaterial->PipelineLayout, nullptr);
	};


	GraphicsPipelineAllocInfo buildPipelineAllocInfo(DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo{};
		l_graphicsPipelineAllocInfo.VertexShader = &p_defaultMaterial->VertexShader;
		l_graphicsPipelineAllocInfo.FragmentShader = &p_defaultMaterial->FragmentShader;
		l_graphicsPipelineAllocInfo.PipelineLayout = p_defaultMaterial->PipelineLayout;
		l_graphicsPipelineAllocInfo.VertexInput = &p_defaultMaterial->VertexInput;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.Device = &p_render->Device;
		l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.SwapChain = &p_render->SwapChain;
		return l_graphicsPipelineAllocInfo;
	};





	void DefaultMaterialInstance_alloc(DefaultMaterialInstance* p_defaultMaterialInstance, DefaultMaterial* p_defaultMaterial, Render* p_render)
	{
		p_defaultMaterialInstance->_DefaultMaterial = p_defaultMaterial;

		// Descriptor set
		_Render::BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.Size = sizeof(_Render::ModelProjection);
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		_Render::VulkanBuffer_alloc(&p_defaultMaterialInstance->ModelProjectionBuffer, &l_bufferAllocInfo, &p_render->Device);

		VkDescriptorSetAllocateInfo l_descriptorSetAllocateInfo{};
		l_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		l_descriptorSetAllocateInfo.descriptorPool = p_defaultMaterialInstance->_DefaultMaterial->DescriptorPool.DescriptorPool;
		l_descriptorSetAllocateInfo.descriptorSetCount = 1;
		l_descriptorSetAllocateInfo.pSetLayouts = &p_defaultMaterialInstance->_DefaultMaterial->DescriptorSetLayout.DescriptorSetLayout;
		vkAllocateDescriptorSets(p_render->Device.LogicalDevice.LogicalDevice, &l_descriptorSetAllocateInfo, &p_defaultMaterialInstance->ModelProjectionDescriptorSet);

		VkDescriptorBufferInfo l_descriptorUniformBufferInfo{};
		l_descriptorUniformBufferInfo.buffer = p_defaultMaterialInstance->ModelProjectionBuffer.Buffer;
		l_descriptorUniformBufferInfo.offset = 0;
		l_descriptorUniformBufferInfo.range = p_defaultMaterialInstance->ModelProjectionBuffer.BufferAllocInfo.Size;

		VkWriteDescriptorSet l_descriptorUniforBufferWrite{};
		l_descriptorUniforBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_descriptorUniforBufferWrite.dstSet = p_defaultMaterialInstance->ModelProjectionDescriptorSet;
		l_descriptorUniforBufferWrite.dstBinding = 0;
		l_descriptorUniforBufferWrite.dstArrayElement = 0;
		l_descriptorUniforBufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorUniforBufferWrite.descriptorCount = 1;
		l_descriptorUniforBufferWrite.pBufferInfo = &l_descriptorUniformBufferInfo;
		l_descriptorUniforBufferWrite.pImageInfo = nullptr;
		l_descriptorUniforBufferWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(p_render->Device.LogicalDevice.LogicalDevice, 1, &l_descriptorUniforBufferWrite, 0, nullptr);
	};

	void DefaultMaterialInstance_free(DefaultMaterialInstance* p_defaultMaterialInstance, Render* p_render)
	{
		vkFreeDescriptorSets(p_render->Device.LogicalDevice.LogicalDevice, p_defaultMaterialInstance->_DefaultMaterial->DescriptorPool.DescriptorPool, 1, &p_defaultMaterialInstance->ModelProjectionDescriptorSet);
		_Render::VulkanBuffer_free(&p_defaultMaterialInstance->ModelProjectionBuffer, &p_render->Device);
	};

	void DefaultMaterialInsance_pushModelProjectionToGPU(DefaultMaterialInstance* p_defaultMaterialInstance, ModelProjection* p_modelProjection, Render* p_render)
	{
		VulkanBuffer_pushToGPU(&p_defaultMaterialInstance->ModelProjectionBuffer, &p_render->Device, p_modelProjection, sizeof(ModelProjection));
	};
};