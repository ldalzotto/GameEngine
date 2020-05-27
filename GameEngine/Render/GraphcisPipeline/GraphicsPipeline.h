#pragma once

#include <vector>
#include <string>
#include "Utils/Algorithm/Algorithm.h"

#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/GraphcisPipeline/FrameBuffer.h"
#include "Render/Shader/Shader.h"
#include "Render/Descriptor/DescriptorPool.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct SwapChain;
	struct GraphcisPipelineContainer;
	struct ShaderContainer;
}

namespace _GameEngine::_Render
{
	struct GraphicsPipelineDependencies
	{
		Device* Device;
		SwapChain* SwapChain;
		GraphcisPipelineContainer* GraphcisPipelineContainer;
		ShaderContainer* ShaderContainer;
	};

	struct GraphicsPipelineKey
	{
		std::string VertexShaderPath;
		std::string FragmentShaderPath;

		bool operator==(const GraphicsPipelineKey& other) const
		{
			return (VertexShaderPath == other.VertexShaderPath
				&& FragmentShaderPath == other.FragmentShaderPath);
		}
	};

	struct GraphicsPipelineKeyHasher
	{
		std::size_t operator()(const GraphicsPipelineKey& Key) const
		{
			size_t l_hash = 0;
			_Utils::Hash_combine(l_hash, Key.VertexShaderPath);
			_Utils::Hash_combine(l_hash, Key.FragmentShaderPath);
			return l_hash;
		}
	};


	struct GraphicsPipeline
	{
		Shader* VertexShader;
		Shader* FragmentShader;

		GraphicsPipelineDependencies GraphicsPipelineDependencies;
		VkPipeline Pipeline;
		VkPipelineLayout PipelineLayout;
		ShaderInputDescription ShaderInputDescription;
		DescriptorPool DescriptorPool;
		RenderPass RenderPass;

		/**
			The number of @ref FrameBuffers is equivalent to the number of images contained in the @ref SwapChain.
		*/
		std::vector<FrameBuffer> FrameBuffers;
		int UsageCounter;
	};

	struct GraphicsPipelineGetOrAllocInfo
	{
		GraphicsPipelineDependencies GraphicsPipelineDependencies;
		GraphicsPipelineKey GraphicsPipelineKey;
	};

	GraphicsPipeline* GraphicsPipeline_getOrAlloc(GraphicsPipelineGetOrAllocInfo* p_graphicsPipelineGetOrAllocInfo);
	void GraphicsPipeline_releaseOrFree(GraphicsPipeline** p_graphicsPipeline);

	void GraphicsPipeline_reallocatePiepeline(GraphicsPipeline* p_graphicsPipeline, SwapChain* p_swapChain);
}