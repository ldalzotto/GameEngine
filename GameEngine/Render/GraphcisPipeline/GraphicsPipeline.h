#pragma once

#include "Render/Device/Device.h"

#include "Render/Shader/Shader.h"

namespace _GameEngine::_Render::_GraphicsPipeline
{
	struct GraphicsPipelineDependencies
	{
		_Device::Device* Device;
	};

	struct GraphicsPieline
	{

	};

	void build(GraphicsPieline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies);

	VkPipelineShaderStageCreateInfo buildShaderStageCreate(_Shader::Shader* p_shader);
}