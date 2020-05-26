#pragma once


#include <vector>

#include "Render/Hardware/Device/Device.h"


#include "Render/SwapChain/SwapChain.h"

#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/GraphcisPipeline/FrameBuffer.h"
#include "Render/Shader/Shader.h"

namespace _GameEngine::_Render
{
	struct GraphicsPipelineDependencies
	{
		Device* Device;
		SwapChain* SwapChain;
	};

	struct GraphicsPipeline
	{
		GraphicsPipelineDependencies GraphicsPipelineDependencies;
		VkPipeline Pipeline;


		
		VkPipelineLayout PipelineLayout;

		ShaderInputDescription ShaderInputDescription;

		RenderPass RenderPass;

		/**
			The number of @ref FrameBuffers is equivalent to the number of images contained in the 
			@ref SwapChain associated to the @ref RenderPass.
		*/
		std::vector<FrameBuffer> FrameBuffers;
	};

	void GraphicsPipeline_build(GraphicsPipeline* p_graphicsPipeline, const GraphicsPipelineDependencies& p_graphicsPipelineDependencies);
	void GraphicsPipeline_free(GraphicsPipeline* p_graphicsPipeline);

}