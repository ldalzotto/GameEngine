#pragma once

#include <vector>

#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

#include "Render/SwapChain/SwapChain.h"

#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/GraphcisPipeline/FrameBuffer.h"
#include "Render/GraphcisPipeline/Shader/Shader.h"

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

		_Shader::VertexShaderDescription VertexShaderDescription;

		RenderPass RenderPass;

		/**
			The number of @ref FrameBuffers is equivalent to the number of images contained in the 
			@ref SwapChain associated to the @ref RenderPass.
		*/
		std::vector<FrameBuffer> FrameBuffers;
	};

	void build(GraphicsPipeline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies);
	void GraphicsPipeline_free(GraphicsPipeline* p_graphicsPipeline);

}