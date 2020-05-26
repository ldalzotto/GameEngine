#include "GraphicsPipeline.h"

#include <stdexcept>

#include "vulkan/vulkan.h"
#include "Log/Log.h"

namespace _GameEngine::_Render
{
	VkPipelineVertexInputStateCreateInfo createVertexInputState(GraphicsPipeline* p_graphicsPipeline);
	VkPipelineInputAssemblyStateCreateInfo creteInputAssemblyState(GraphicsPipeline* p_graphcisPipeline);
	VkViewport createViewport(GraphicsPipeline* p_graphicsPipeline);
	VkRect2D createScissor(GraphicsPipeline* p_graphicsPipeline);
	VkPipelineViewportStateCreateInfo createViewportState(GraphicsPipeline* p_graphicsPipeline, VkViewport* p_viewPort, VkRect2D* p_scissor);
	VkPipelineRasterizationStateCreateInfo createRasterizationState(GraphicsPipeline* p_graphicsPipeline);
	VkPipelineMultisampleStateCreateInfo createMultisampleState(GraphicsPipeline* p_graphicsPipeline);
	VkPipelineColorBlendAttachmentState createColorBlendAttachmentState(GraphicsPipeline* p_graphicsPipeline);
	VkPipelineColorBlendStateCreateInfo createColorBlendState(GraphicsPipeline* p_graphicsPipeline, VkPipelineColorBlendAttachmentState* p_colorBlendAttachmentState);

	void createPipelineLayout(GraphicsPipeline* p_graphicsPipeline);
	void clearPipelineLayout(GraphicsPipeline* p_graphcisPipeline);

	void GraphicsPipeline_build(GraphicsPipeline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies)
	{
		p_graphicsPipeline->GraphicsPipelineDependencies = p_graphicsPipelineDependencies;

		ShaderDependencies l_shaderDependencies;
		l_shaderDependencies.Device = p_graphicsPipelineDependencies.Device;

		Shader l_vertexShader = Shader_create(l_shaderDependencies, ShaderType::VERTEX, "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialVertex.spv");
		VertexInput_buildInput(&p_graphicsPipeline->ShaderInputDescription.VertexInput);
		DescriptorSetLayout_alloc(&p_graphicsPipeline->ShaderInputDescription.DescriptorSetLayout, p_graphicsPipelineDependencies.Device);

		Shader l_fragmentShader = Shader_create(l_shaderDependencies, ShaderType::FRAGMENT, "G:/GameProjects/VulkanTutorial/Assets/Shader/out/TutorialFragment.spv");

		VkPipelineShaderStageCreateInfo l_shaderStages[] = { Shader_buildShaderStageCreate(&l_vertexShader), Shader_buildShaderStageCreate(&l_fragmentShader) };

		VkGraphicsPipelineCreateInfo l_pipelineCreateInfo{};
		l_pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		l_pipelineCreateInfo.stageCount = 2;
		l_pipelineCreateInfo.pStages = l_shaderStages;

		auto l_vertexInputState = createVertexInputState(p_graphicsPipeline);
		auto l_inputAssemblyState = creteInputAssemblyState(p_graphicsPipeline);
		auto l_viewport = createViewport(p_graphicsPipeline);
		auto l_scissor = createScissor(p_graphicsPipeline);
		auto l_viewportState = createViewportState(p_graphicsPipeline, &l_viewport, &l_scissor);
		auto l_rasterizationState = createRasterizationState(p_graphicsPipeline);
		auto l_multisampleState = createMultisampleState(p_graphicsPipeline);
		auto l_colorBlendAttachmentState = createColorBlendAttachmentState(p_graphicsPipeline);
		auto l_colorBlendState = createColorBlendState(p_graphicsPipeline, &l_colorBlendAttachmentState);

		l_pipelineCreateInfo.pVertexInputState = &l_vertexInputState;
		l_pipelineCreateInfo.pInputAssemblyState = &l_inputAssemblyState;
		l_pipelineCreateInfo.pViewportState = &l_viewportState;
		l_pipelineCreateInfo.pRasterizationState = &l_rasterizationState;
		l_pipelineCreateInfo.pMultisampleState = &l_multisampleState;
		l_pipelineCreateInfo.pColorBlendState = &l_colorBlendState;

		createPipelineLayout(p_graphicsPipeline);
		l_pipelineCreateInfo.layout = p_graphicsPipeline->PipelineLayout;


		RenderPassBuildInfo l_renderpassBuildInfo{};
		RenderPassDependencies l_renderPassDependencies{};
		l_renderPassDependencies.SwapChain = p_graphicsPipelineDependencies.SwapChain;
		l_renderpassBuildInfo.RenderPassDependencies = &l_renderPassDependencies;
		RenderPass_build(&p_graphicsPipeline->RenderPass, &l_renderpassBuildInfo);

		l_pipelineCreateInfo.renderPass = p_graphicsPipeline->RenderPass.renderPass;
		l_pipelineCreateInfo.subpass = 0;
		l_pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		l_pipelineCreateInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(p_graphicsPipeline->GraphicsPipelineDependencies.Device->LogicalDevice.LogicalDevice, VK_NULL_HANDLE, 1, &l_pipelineCreateInfo, nullptr, &p_graphicsPipeline->Pipeline)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create graphics pipeline!"));
		}

		Shader_free(&l_vertexShader);
		Shader_free(&l_fragmentShader);


		std::vector<SwapChainImage>* l_swapChainImages = &p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainImages;
		p_graphicsPipeline->FrameBuffers.resize(l_swapChainImages->size());

		for (size_t i = 0; i < p_graphicsPipeline->FrameBuffers.size(); i++)
		{
			FrameBufferDependencies l_frameBufferDependencies{};
			l_frameBufferDependencies.RenderPass = &p_graphicsPipeline->RenderPass;
			l_frameBufferDependencies.ImageView = &l_swapChainImages->at(i).ImageView;
			l_frameBufferDependencies.Device = p_graphicsPipeline->GraphicsPipelineDependencies.Device;
			l_frameBufferDependencies.SwapChainInfo = &p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainInfo;
			FrameBuffer_init(&p_graphicsPipeline->FrameBuffers[i], &l_frameBufferDependencies);
		}
	};

	void GraphicsPipeline_free(GraphicsPipeline* p_graphicsPipeline)
	{
		DescriptorSetLayout_free(&p_graphicsPipeline->ShaderInputDescription.DescriptorSetLayout, p_graphicsPipeline->GraphicsPipelineDependencies.Device);

		for (size_t i = 0; i < p_graphicsPipeline->FrameBuffers.size(); i++)
		{
			FrameBuffer_free(&p_graphicsPipeline->FrameBuffers[i]);
		}

		vkDestroyPipeline(p_graphicsPipeline->GraphicsPipelineDependencies.Device->LogicalDevice.LogicalDevice, p_graphicsPipeline->Pipeline, nullptr);
		clearPipelineLayout(p_graphicsPipeline);
		RenderPass_free(&p_graphicsPipeline->RenderPass);
	};

	VkPipelineVertexInputStateCreateInfo createVertexInputState(GraphicsPipeline* p_graphicsPipeline)
	{
		VkPipelineVertexInputStateCreateInfo l_vertexInputStateCreate{};
		l_vertexInputStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		l_vertexInputStateCreate.vertexBindingDescriptionCount = 1;
		l_vertexInputStateCreate.pVertexBindingDescriptions = &p_graphicsPipeline->ShaderInputDescription.VertexInput.VertexInputBinding;
		l_vertexInputStateCreate.vertexAttributeDescriptionCount = static_cast<uint32_t>(p_graphicsPipeline->ShaderInputDescription.VertexInput.VertexInputAttributeDescriptions.size());
		l_vertexInputStateCreate.pVertexAttributeDescriptions = p_graphicsPipeline->ShaderInputDescription.VertexInput.VertexInputAttributeDescriptions.data();
		return l_vertexInputStateCreate;
	};

	VkPipelineInputAssemblyStateCreateInfo creteInputAssemblyState(GraphicsPipeline* p_graphcisPipeline)
	{
		VkPipelineInputAssemblyStateCreateInfo l_inputAssemblyState{};
		l_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		l_inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		l_inputAssemblyState.primitiveRestartEnable = VK_FALSE;
		return l_inputAssemblyState;
	};

	VkViewport createViewport(GraphicsPipeline* p_graphicsPipeline)
	{
		VkViewport l_viewport{};
		l_viewport.x = 0.0f;
		l_viewport.y = 0.0f;
		l_viewport.width = (float)p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainInfo.SwapExtend.width;
		l_viewport.height = (float)p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainInfo.SwapExtend.height;
		l_viewport.minDepth = 0.0f;
		l_viewport.maxDepth = 1.0f;
		return l_viewport;
	};

	VkRect2D createScissor(GraphicsPipeline* p_graphicsPipeline)
	{
		VkRect2D l_scissor{};
		l_scissor.offset = { 0,0 };
		l_scissor.extent = p_graphicsPipeline->GraphicsPipelineDependencies.SwapChain->SwapChainInfo.SwapExtend;
		return l_scissor;
	};

	VkPipelineViewportStateCreateInfo createViewportState(GraphicsPipeline* p_graphicsPipeline, VkViewport* p_viewPort, VkRect2D* p_scissor)
	{
		VkPipelineViewportStateCreateInfo l_viewportState{};
		l_viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		l_viewportState.viewportCount = 1;
		l_viewportState.pViewports = p_viewPort;
		l_viewportState.scissorCount = 1;
		l_viewportState.pScissors = p_scissor;
		return l_viewportState;
	};

	VkPipelineRasterizationStateCreateInfo createRasterizationState(GraphicsPipeline* p_graphicsPipeline)
	{
		VkPipelineRasterizationStateCreateInfo l_rasterizationState{};
		l_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		l_rasterizationState.depthClampEnable = VK_FALSE;
		l_rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		l_rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
		l_rasterizationState.lineWidth = 1.0f;
		l_rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
		l_rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		l_rasterizationState.depthBiasEnable = VK_FALSE;
		l_rasterizationState.depthBiasConstantFactor = 0.0f;
		l_rasterizationState.depthBiasClamp = 0.0f;
		l_rasterizationState.depthBiasSlopeFactor = 0.0f;
		return l_rasterizationState;
	};

	VkPipelineMultisampleStateCreateInfo createMultisampleState(GraphicsPipeline* p_graphicsPipeline)
	{
		VkPipelineMultisampleStateCreateInfo l_multisampleStateCreateInfo{};
		l_multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		l_multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
		l_multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		l_multisampleStateCreateInfo.minSampleShading = 1.0f;
		l_multisampleStateCreateInfo.pSampleMask = nullptr;
		l_multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
		l_multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
		return l_multisampleStateCreateInfo;
	};

	VkPipelineColorBlendAttachmentState createColorBlendAttachmentState(GraphicsPipeline* p_graphicsPipeline)
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
		return l_colorBlendAttachmentState;
	};

	VkPipelineColorBlendStateCreateInfo createColorBlendState(GraphicsPipeline* p_graphicsPipeline, VkPipelineColorBlendAttachmentState* p_colorBlendAttachmentState)
	{
		VkPipelineColorBlendStateCreateInfo l_colorBlendStateCreate{};
		l_colorBlendStateCreate.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		l_colorBlendStateCreate.logicOpEnable = VK_FALSE;
		l_colorBlendStateCreate.logicOp = VK_LOGIC_OP_COPY;
		l_colorBlendStateCreate.attachmentCount = 1;
		l_colorBlendStateCreate.pAttachments = p_colorBlendAttachmentState;
		l_colorBlendStateCreate.blendConstants[0] = 0.0f;
		l_colorBlendStateCreate.blendConstants[1] = 0.0f;
		l_colorBlendStateCreate.blendConstants[2] = 0.0f;
		l_colorBlendStateCreate.blendConstants[3] = 0.0f;
		return l_colorBlendStateCreate;
	};

	void createPipelineLayout(GraphicsPipeline* p_graphicsPipeline)
	{
		VkPipelineLayoutCreateInfo l_pipelineLayoutCreateInfo{};
		l_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		l_pipelineLayoutCreateInfo.setLayoutCount = 1;
		l_pipelineLayoutCreateInfo.pSetLayouts = &p_graphicsPipeline->ShaderInputDescription.DescriptorSetLayout.DescriptorSetLayout;
		l_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
		l_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(p_graphicsPipeline->GraphicsPipelineDependencies.Device->LogicalDevice.LogicalDevice, &l_pipelineLayoutCreateInfo, nullptr, &p_graphicsPipeline->PipelineLayout)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create pipeline layout!"));
		}
	};

	void clearPipelineLayout(GraphicsPipeline* p_graphicsPipeline)
	{
		vkDestroyPipelineLayout(p_graphicsPipeline->GraphicsPipelineDependencies.Device->LogicalDevice.LogicalDevice, p_graphicsPipeline->PipelineLayout, nullptr);
	};


}