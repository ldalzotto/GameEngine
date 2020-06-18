#pragma once

#include <vector>
#include <string>
#include "Utils/Algorithm/Algorithm.h"
#include "Utils/Functional/OptionalT.h"

#include "VulkanObjects/GraphcisPipeline/RenderPass.h"
#include "VulkanObjects/GraphcisPipeline/FrameBuffer.h"
#include "VulkanObjects/Descriptor/DescriptorPool.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct SwapChain;
	struct Shader;
	struct VertexInput;
	struct Texture;
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

	struct GraphicsPipeline_DepthTest
	{
		Texture* DepthTexture;
	};

	struct GraphicsPipelineAllocInfo
	{
		GraphicsPipelineDependencies GraphicsPipelineDependencies;
		Shader* VertexShader;
		Shader* FragmentShader;
		VertexInput* VertexInput;
		VkPipelineLayout PipelineLayout;
		GraphicsPipeline_DepthTest GraphicsPipeline_DepthTest;

		/** OPTIONAL - Default value is VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST */
		_Utils::OptionalT<VkPrimitiveTopology> PrimitiveTopology;
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