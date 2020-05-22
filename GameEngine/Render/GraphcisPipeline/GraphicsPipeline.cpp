#include "GraphicsPipeline.h"

#include <stdexcept>

#include "vulkan/vulkan.h"
#include "Log/Log.h"

namespace _GameEngine::_Render::_GraphicsPipeline
{

	VkPipelineVertexInputStateCreateInfo createVertexInputState(GraphicsPieline* p_graphicsPipeline);
	VkPipelineInputAssemblyStateCreateInfo creteInputAssemblyState(GraphicsPieline* p_graphcisPipeline);
	VkPipelineViewportStateCreateInfo createViewportState(GraphicsPieline* p_graphicsPipeline);
	VkPipelineRasterizationStateCreateInfo createRasterizationState(GraphicsPieline* p_graphicsPipeline);
	VkPipelineMultisampleStateCreateInfo createMultisampleState(GraphicsPieline* p_graphicsPipeline);
	VkPipelineColorBlendStateCreateInfo createColorBlendState(GraphicsPieline* p_graphicsPipeline);

	void createPipelineLayout(GraphicsPieline* p_graphicsPipeline);
	void clearPipelineLayout(GraphicsPieline* p_graphcisPipeline);

	void build(GraphicsPieline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies)
	{
		p_graphicsPipeline->GraphicsPipelineDependencies = p_graphicsPipelineDependencies;

		_Shader::ShaderDependencies l_shaderDependencies;
		l_shaderDependencies.Device = p_graphicsPipelineDependencies.Device;

		_Shader::Shader l_vertexShader = _Shader::createShader(l_shaderDependencies, _Shader::ShaderType::VERTEX, "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialVertex.spv");
		_Shader::Shader l_fragmentShader = _Shader::createShader(l_shaderDependencies, _Shader::ShaderType::FRAGMENT, "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialFragment.spv");

		VkPipelineShaderStageCreateInfo l_shaderStages[] = { buildShaderStageCreate(&l_vertexShader), buildShaderStageCreate(&l_fragmentShader) };

		_Shader::freeShader(&l_vertexShader);
		_Shader::freeShader(&l_fragmentShader);

	};

	VkPipelineShaderStageCreateInfo buildShaderStageCreate(_Shader::Shader* p_shader)
	{
		VkPipelineShaderStageCreateInfo l_pipelineShaderStageCreate{};
		l_pipelineShaderStageCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

		if (p_shader->ShaderType == _Shader::ShaderType::VERTEX)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_VERTEX_BIT;
		}
		else if (p_shader->ShaderType == _Shader::ShaderType::FRAGMENT)
		{
			l_pipelineShaderStageCreate.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		l_pipelineShaderStageCreate.module = p_shader->ShaderModule;
		l_pipelineShaderStageCreate.pName = "main";

		return l_pipelineShaderStageCreate;
	};

	VkPipelineVertexInputStateCreateInfo createVertexInputState(GraphicsPieline* p_graphicsPipeline)
	{
		VkPipelineVertexInputStateCreateInfo l_vertexInputStateCreate{};
		l_vertexInputStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		l_vertexInputStateCreate.vertexBindingDescriptionCount = 0;
		l_vertexInputStateCreate.pVertexBindingDescriptions = nullptr;
		l_vertexInputStateCreate.vertexAttributeDescriptionCount = 0;
		l_vertexInputStateCreate.pVertexAttributeDescriptions = nullptr;
		return l_vertexInputStateCreate;
	};

	VkPipelineInputAssemblyStateCreateInfo creteInputAssemblyState(GraphicsPieline* p_graphcisPipeline)
	{
		VkPipelineInputAssemblyStateCreateInfo l_inputAssemblyState{};
		l_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		l_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		l_inputAssemblyState.primitiveRestartEnable = VK_FALSE;
		return l_inputAssemblyState;
	};

	VkPipelineViewportStateCreateInfo createViewportState(GraphicsPieline* p_graphicsPipeline)
	{
		VkViewport l_viewport{};
		l_viewport.x = 0.0f;
		l_viewport.y = 0.0f;
		l_viewport.width = (float)p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainInfo.SwapExtend.width;
		l_viewport.height = (float)p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainInfo.SwapExtend.height;
		l_viewport.minDepth = 0.0f;
		l_viewport.maxDepth = 1.0f;

		VkRect2D l_scissor{};
		l_scissor.offset = { 0,0 };
		l_scissor.extent = p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainInfo.SwapExtend;

		VkPipelineViewportStateCreateInfo l_viewportState{};
		l_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		l_viewportState.viewportCount = 1;
		l_viewportState.pViewports = &l_viewport;
		l_viewportState.scissorCount = 1;
		l_viewportState.pScissors = &l_scissor;
		return l_viewportState;
	};

	VkPipelineRasterizationStateCreateInfo createRasterizationState(GraphicsPieline* p_graphicsPipeline)
	{
		VkPipelineRasterizationStateCreateInfo l_rasterizationState{};
		l_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		l_rasterizationState.depthClampEnable = VK_FALSE;
		l_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		l_rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		l_rasterizationState.lineWidth = 1.0f;
		l_rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
		l_rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
		l_rasterizationState.depthBiasEnable = VK_FALSE;
		l_rasterizationState.depthBiasConstantFactor = 0.0f;
		l_rasterizationState.depthBiasClamp = 0.0f;
		l_rasterizationState.depthBiasSlopeFactor = 0.0f;
		return l_rasterizationState;
	};

	VkPipelineMultisampleStateCreateInfo createMultisampleState(GraphicsPieline* p_graphicsPipeline)
	{
		VkPipelineMultisampleStateCreateInfo l_multisampleStateCreateInfo;
		l_multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		l_multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
		l_multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		l_multisampleStateCreateInfo.minSampleShading = 1.0f;
		l_multisampleStateCreateInfo.pSampleMask = nullptr;
		l_multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
		l_multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
		return l_multisampleStateCreateInfo;
	};

	VkPipelineColorBlendStateCreateInfo createColorBlendState(GraphicsPieline* p_graphicsPipeline)
	{
		VkPipelineColorBlendAttachmentState l_colorBlendAttachmentState{};
		l_colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		l_colorBlendAttachmentState.blendEnable = VK_FALSE;
		l_colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		l_colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		l_colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
		l_colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		l_colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		l_colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo l_colorBlendStateCreate{};
		l_colorBlendStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		l_colorBlendStateCreate.logicOpEnable = VK_FALSE;
		l_colorBlendStateCreate.logicOp = VK_LOGIC_OP_COPY;
		l_colorBlendStateCreate.attachmentCount = 1;
		l_colorBlendStateCreate.pAttachments = &l_colorBlendAttachmentState;
		l_colorBlendStateCreate.blendConstants[0] = 0.0f;
		l_colorBlendStateCreate.blendConstants[1] = 0.0f;
		l_colorBlendStateCreate.blendConstants[2] = 0.0f;
		l_colorBlendStateCreate.blendConstants[3] = 0.0f;
		return l_colorBlendStateCreate;
	};

	void createPipelineLayout(GraphicsPieline* p_graphicsPipeline)
	{
		VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
		l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		l_pipelineLayoutCreateInfo.setLayoutCount = 0;
		l_pipelineLayoutCreateInfo.pSetLayouts = nullptr;
		l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(p_graphicsPipeline->GraphicsPipelineDependencies.Device->LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &p_graphicsPipeline->PipelineLayout)
					!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create pipeline layout!"));
		}
	};

	void clearPipelineLayout(GraphicsPieline* p_graphicsPipeline)
	{
		vkDestroyPipelineLayout(p_graphicsPipeline->GraphicsPipelineDependencies.Device->LogicalDevice.LogicalDevice, p_graphicsPipeline->PipelineLayout, nullptr);
		RenderPass_free(&p_graphicsPipeline->RenderPass);
	};
}