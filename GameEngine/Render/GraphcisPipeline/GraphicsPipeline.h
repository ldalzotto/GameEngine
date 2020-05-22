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

	struct GraphicsPieline
	{
		GraphicsPipelineDependencies GraphicsPipelineDependencies;
		VkPipelineLayout PipelineLayout;
		RenderPass RenderPass;
	};

	void build(GraphicsPieline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies);

	VkPipelineShaderStageCreateInfo buildShaderStageCreate(_Shader::Shader* p_shader);
}