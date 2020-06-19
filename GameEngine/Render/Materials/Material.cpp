#include "Material.h"

#include <array>
#include <stdexcept>

#include "Utils/Algorithm/Algorithm.h"
#include "Log/Log.h"

#include "RenderInterface.h"
#include "VulkanObjects/Hardware/Device/Device.h"
#include "LoopStep/CameraBufferSetupStep.h"


namespace _GameEngine::_Render
{

	void createDescriptorSetLayout(Material_LocalInputParameters* p_localInputParameters, RenderInterface* p_renderInterface);
	void freeDescriptorSetLayout(DescriptorSetLayout* p_descriptorSetLayout, RenderInterface* p_renderInterface);

	void createDescriptorPool(DescriptorPool* p_descriptorPool, DescriptorSetLayout* p_descriptorSetLayout, RenderInterface* p_renderInterface);
	void freeDescriptorPool(Material* p_defaultMaterial, RenderInterface* p_renderInterface);

	void createPipelineLayout(Material* p_defaultMaterial, RenderInterface* p_renderInterface);
	void freePipelineLayout(Material* p_defaultMaterial, RenderInterface* p_renderInterface);

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(Material* p_defaultMaterial, RenderInterface* p_renderInterface);

	size_t MaterialUniqueKey_buildHash(MaterialUniqueKey* p_materialUniqueKey)
	{
		size_t l_hash = 0;
		_Utils::Hash_combine(l_hash, p_materialUniqueKey->VertexShaderPath);
		_Utils::Hash_combine(l_hash, p_materialUniqueKey->FragmentShaderPath);
		return l_hash;
	};

	Material* Material_alloc(RenderInterface* p_renderInterface, MaterialAllocInfo* p_materialAllocInfo)
	{

		Material* l_material = new Material();

		l_material->RenderingOrder = p_materialAllocInfo->RenderingOrder;
		l_material->Configuration.PrimitiveTopologyOverride = p_materialAllocInfo->PrimitiveTopologyOverride;
		l_material->FinalDrawObjects.MaterialDrawFn = p_materialAllocInfo->MaterialDrawFn;
		l_material->MaterialUniqueKey.VertexShaderPath = p_materialAllocInfo->VertexShader;
		l_material->MaterialUniqueKey.FragmentShaderPath = p_materialAllocInfo->FragmentShader;

		l_material->LocalInputParameters.ShaderParameters = p_materialAllocInfo->ShaderParameters;

		if (p_materialAllocInfo->UseDepthBuffer)
		{
			l_material->InternalResources.DepthBufferTexture = *p_renderInterface->DepthTexture;
		}

		{
			Material_ExternalResources* l_externalResources = &l_material->ExternalResources;
			l_externalResources->VertexShader.ShaderPath = p_materialAllocInfo->VertexShader;
			l_externalResources->VertexShader.ShaderType = ShaderType::VERTEX;

			l_externalResources->FragmentShader.ShaderPath = p_materialAllocInfo->FragmentShader;
			l_externalResources->FragmentShader.ShaderType = ShaderType::FRAGMENT;
		}

		{
			auto l_localInputParameters = &l_material->LocalInputParameters;
			l_localInputParameters->VertexInput = p_materialAllocInfo->VertexInput;
			createDescriptorSetLayout(l_localInputParameters, p_renderInterface);
			createDescriptorPool(&l_localInputParameters->DescriptorPool, &l_localInputParameters->DescriptorSetLayout, p_renderInterface);
		}

		{
			auto l_finalDrawObjects = &l_material->FinalDrawObjects;
			createPipelineLayout(l_material, p_renderInterface);
			GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(l_material, p_renderInterface);
			GraphicsPipeline_alloc(&l_finalDrawObjects->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
		}

		return l_material;
	}

	void Material_free(Material** p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		Material* l_material = *p_defaultMaterial;

		for (ShaderParameter& l_shaderParameter : l_material->LocalInputParameters.ShaderParameters)
		{
			ShaderParameter_free(&l_shaderParameter);
		}
		l_material->LocalInputParameters.ShaderParameters.clear();

		GraphicsPipeline_free(&l_material->FinalDrawObjects.GraphicsPipeline);
		freePipelineLayout(l_material, p_renderInterface);
		freeDescriptorPool(l_material, p_renderInterface);
		freeDescriptorSetLayout(&l_material->LocalInputParameters.DescriptorSetLayout, p_renderInterface);

		delete l_material;
		l_material = nullptr;
	};

	void Material_reAllocGraphicsPipeline(Material* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		p_defaultMaterial->InternalResources.DepthBufferTexture = *p_renderInterface->DepthTexture;

		GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo = buildPipelineAllocInfo(p_defaultMaterial, p_renderInterface);
		GraphicsPipeline_reallocatePipeline(&p_defaultMaterial->FinalDrawObjects.GraphicsPipeline, &l_graphicsPipelineAllocInfo);
	};

	void createDescriptorSetLayout(Material_LocalInputParameters* p_localInputParameters, RenderInterface* p_renderInterface)
	{
		DescriptorSetLayoutAllocInfo l_descriptorSetLayoutAllocInfo{};

		std::vector<VkDescriptorSetLayoutBinding> l_defaultMaterialDescriptorSetLayoutBindings;
		for (ShaderParameter& l_shaderParameter : p_localInputParameters->ShaderParameters)
		{
			l_defaultMaterialDescriptorSetLayoutBindings.emplace_back(l_shaderParameter.DescriptorSetLayoutBinding);
		}

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

	void freeDescriptorPool(Material* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		DescriptorPool_free(&p_defaultMaterial->LocalInputParameters.DescriptorPool, p_renderInterface->Device);
	};

	void createPipelineLayout(Material* p_defaultMaterial, RenderInterface* p_renderInterface)
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

	void freePipelineLayout(Material* p_defaultMaterial, RenderInterface* p_renderInterface)
	{
		vkDestroyPipelineLayout(p_renderInterface->Device->LogicalDevice.LogicalDevice, p_defaultMaterial->FinalDrawObjects.PipelineLayout, nullptr);
	};

	GraphicsPipelineAllocInfo buildPipelineAllocInfo(Material* p_defaultMaterial, RenderInterface* p_renderInterface)
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

		l_graphicsPipelineAllocInfo.PrimitiveTopology = p_defaultMaterial->Configuration.PrimitiveTopologyOverride;
				
		return l_graphicsPipelineAllocInfo;
	};

}