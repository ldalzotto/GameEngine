#pragma once

#include "Render/SwapChain/SwapChain.h"
#include "vulkan/vulkan.h"

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
	};

	void RenderPass_build(RenderPass* p_renderPass, RenderPassBuildInfo* p_renderPassBuildInfo);
	void RenderPass_free(RenderPass* p_renderPass);
};