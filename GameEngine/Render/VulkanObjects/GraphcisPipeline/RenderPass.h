#pragma once

#include "Includes/VulkanIncludes.h"

namespace _GameEngine::_Render
{
	struct SwapChain;
}

namespace _GameEngine::_Render
{
	struct RenderPassDependencies
	{
		SwapChain* SwapChain;
	};

	struct RenderPass
	{
		RenderPassDependencies RenderPassDependencies;
		VkRenderPass renderPass;
	};

	struct RenderPassBuildInfo
	{
		RenderPassDependencies* RenderPassDependencies;

		VkAttachmentDescription* ColorAttachmentDescription;
		/** If null, the depth testing is not enabled. */
		VkFormat* DepthTextureFormat;
	};

	void RenderPass_build(RenderPass* p_renderPass, RenderPassBuildInfo* p_renderPassBuildInfo);
	void RenderPass_free(RenderPass* p_renderPass);
};