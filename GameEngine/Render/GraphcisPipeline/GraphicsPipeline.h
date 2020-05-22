#pragma once

#include "Render/Device/Device.h"
#include "Render/SwapChain/SwapChain.h"

#include "Render/GraphcisPipeline/RenderPass.h"

#include "Render/Shader/Shader.h"

namespace _GameEngine::_Render::_GraphicsPipeline
{
	struct GraphicsPipelineDependencies
	{
		_Device::Device* Device;
		_SwapChain::SwapChain* SwapChain;
	};

	struct GraphicsPipeline
	{
		GraphicsPipelineDependencies GraphicsPipelineDependencies;
		VkPipeline Pipeline;
		VkPipelineLayout PipelineLayout;
		RenderPass RenderPass;
	};

	void build(GraphicsPipeline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies);
	void GraphicsPipeline_free(GraphicsPipeline* p_graphicsPipeline);

	VkPipelineShaderStageCreateInfo buildShaderStageCreate(_Shader::Shader* p_shader);
}