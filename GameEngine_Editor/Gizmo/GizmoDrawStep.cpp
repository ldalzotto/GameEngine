#include "GizmoDrawStep.h"

#include <array>

#include "Render/Texture/Texture.h"
#include "Render/RenderInterface.h"
#include "Render/Mesh/Mesh.h"
#include "Render/LoopStep/CameraBufferSetupStep.h"
#include "VulkanObjects/Hardware/Device/Device.h"

namespace _GameEngineEditor
{

	void GizmoDrawStep_alloc(GizmoDrawStep* p_gizmoDrawStep, RenderInterface* p_renderInterface)
	{
		GizmoDrawStepParameters* l_parameters = &p_gizmoDrawStep->GizmoDrawStepParameters;
		{
			l_parameters->VertexShader.ShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/EditorVertex.spv";
			l_parameters->VertexShader.ShaderType = ShaderType::VERTEX;

			l_parameters->FragmentShader.ShaderPath = "E:/GameProjects/GameEngine/Assets/Shader/out/EditorFragment.spv";
			l_parameters->FragmentShader.ShaderType = ShaderType::FRAGMENT;
		}

		{
			VertexInput_buildInput(&l_parameters->VertexInput);
		}

		{
			l_parameters->ModelMatrixParameter.Binding = 0;
			l_parameters->ModelMatrixParameter.BufferSize = sizeof(Vertex);
			l_parameters->ModelMatrixParameter.StageFlag = VK_SHADER_STAGE_VERTEX_BIT;
		}

		{
			std::vector<VkDescriptorSetLayoutBinding> l_bindings
			{
				UniformBufferParameter_buildLayoutBinding(&l_parameters->ModelMatrixParameter)
			};
			DescriptorSetLayoutAllocInfo l_descriptorLayourAllocInfo{};
			l_descriptorLayourAllocInfo.LayoutBindings = &l_bindings;
			DescriptorSetLayout_alloc(&l_parameters->DescriptorSetLayout, p_renderInterface->Device, &l_descriptorLayourAllocInfo);
		}

		{
			std::vector<VkDescriptorPoolSize> l_descriptorPoolSize
			{
				{VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000}
			};
			DescriptorPoolAllocInfo l_descriptorPoolAllocInfo{};
			l_descriptorPoolAllocInfo.MaxSet = 1000;
			l_descriptorPoolAllocInfo.SourceDescriptorPoolSize = &l_descriptorPoolSize;
			l_descriptorPoolAllocInfo.DescriptionPoolCreateFlags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			DescriptorPool_alloc(&l_parameters->DescriptorPool, p_renderInterface->Device, &l_descriptorPoolAllocInfo);
		}


		{
			TextureAllocInfo l_textureAllocInfo{};
			l_textureAllocInfo.FitSwapChainSize = true;
			l_textureAllocInfo.TextureAllocationType = TextureAllocationType::PROCEDURAL;
			l_textureAllocInfo.TextureCreateInfo.TextureType = TextureType::DEPTH;
			l_textureAllocInfo.TextureCreateInfo.TextureUsage = TextureUsage::PIPELINE_ATTACHMENT;
			l_textureAllocInfo.TextureKey.TexturePath = "DEPTH";
			l_textureAllocInfo.RenderInterface = p_renderInterface;
			l_parameters->OwnDepthTexture = Texture_alloc(&l_textureAllocInfo);
		}

		GizmoDrawStepDrawObject* l_drawObjects = &p_gizmoDrawStep->GizmoDrawStepDrawObject;

		{
			std::array<VkDescriptorSetLayout, 2> l_descriptorSetLayouts = {
					p_renderInterface->CameraBufferSetupStep->DescriptorSetLayout.DescriptorSetLayout,
					l_parameters->DescriptorSetLayout.DescriptorSetLayout
			};

			VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
			l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			l_pipelineLayoutCreateInfo.setLayoutCount = static_cast<uint32_t>(l_descriptorSetLayouts.size());
			l_pipelineLayoutCreateInfo.pSetLayouts = l_descriptorSetLayouts.data();
			l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

			vkCreatePipelineLayout(p_renderInterface->Device->LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &l_drawObjects->PipelineLayout);
		}

		{
			GraphicsPipelineAllocInfo l_graphicsPipelineAllocInfo{};
			l_graphicsPipelineAllocInfo.FragmentShader = &l_parameters->FragmentShader;
			l_graphicsPipelineAllocInfo.VertexShader = &l_parameters->VertexShader;
			l_graphicsPipelineAllocInfo.PipelineLayout = l_drawObjects->PipelineLayout;
			l_graphicsPipelineAllocInfo.VertexInput = &l_parameters->VertexInput;
			l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.Device = p_renderInterface->Device;
			l_graphicsPipelineAllocInfo.GraphicsPipelineDependencies.SwapChain = p_renderInterface->SwapChain;
			l_graphicsPipelineAllocInfo.GraphicsPipeline_DepthTest.DepthTexture = l_parameters->OwnDepthTexture;
			GraphicsPipeline_alloc(&l_drawObjects->GraphicsPipeline, &l_graphicsPipelineAllocInfo);
		}
	};
	
	void GizmoDrawStep_free(GizmoDrawStep* p_gizmoDrawStep, RenderInterface* p_renderInterface)
	{
		DescriptorSetLayout_free(&p_gizmoDrawStep->GizmoDrawStepParameters.DescriptorSetLayout, p_renderInterface->Device);
		DescriptorPool_free(&p_gizmoDrawStep->GizmoDrawStepParameters.DescriptorPool, p_renderInterface->Device);
		vkDestroyPipelineLayout(p_renderInterface->Device->LogicalDevice.LogicalDevice, p_gizmoDrawStep->GizmoDrawStepDrawObject.PipelineLayout, nullptr);
		Texture_free(&p_gizmoDrawStep->GizmoDrawStepParameters.OwnDepthTexture, p_renderInterface);
		GraphicsPipeline_free(&p_gizmoDrawStep->GizmoDrawStepDrawObject.GraphicsPipeline);
	};
}