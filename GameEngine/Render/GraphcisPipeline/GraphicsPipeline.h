#pragma once

#include <vector>
#include <string>
#include "Utils/Algorithm/Algorithm.h"

#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/GraphcisPipeline/FrameBuffer.h"
#include "Render/Descriptor/DescriptorPool.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct SwapChain;
	struct Shader;
	struct VertexInput;
}

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

		struct PipelineInternalsType
		{
			VkPipeline Pipeline;
			RenderPass RenderPass;

			/**
				The number of @ref FrameBuffers is equivalent to the number of images contained in the @ref SwapChain.
			*/
			std::vector<FrameBuffer> FrameBuffers;
		};

		PipelineInternalsType PipelineInternals;
	};

	struct GraphicsPipelineAllocInfo
	{
		GraphicsPipelineDependencies GraphicsPipelineDependencies;
		Shader* VertexShader;
		Shader* FragmentShader;
		VertexInput* VertexInput;
		VkPipelineLayout PipelineLayout;
	};

	void GraphicsPipeline_alloc(GraphicsPipeline* p_graphicsPipeline, GraphicsPipelineAllocInfo* p_graphicsPipelineGetOrAllocInfo);
	void GraphicsPipeline_free(GraphicsPipeline* p_graphicsPipeline);

	/**
		Reallocate internal pipeline objects with the new input @ref SwapChain.
		This method must be called when the @ref SwapChain has changed because the pipeline is bounded
		to informations like texture size.
		Thus, we reallocate the pipeline to reflect the changes.
	*/
	void GraphicsPipeline_reallocatePipeline(GraphicsPipeline* p_graphicsPipeline, GraphicsPipelineAllocInfo* p_graphicsPipelineGetOrAllocInfo);
}