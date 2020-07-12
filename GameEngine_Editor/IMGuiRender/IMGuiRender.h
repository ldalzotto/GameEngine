#pragma once

#include "vulkan/vulkan.h"

#include <vector>

#include "GameEngineApplicationInterface.h"

extern "C"
{
#include "Functional/Callback/Callback.h"
}

#include "VulkanObjects/GraphcisPipeline/RenderPass.h"
#include "VulkanObjects/GraphcisPipeline/FrameBuffer.h"
#include "VulkanObjects/Descriptor/DescriptorPool.h"

using namespace _GameEngine;

namespace _GameEngine
{
	struct GameEngineApplication;
}

namespace _GameEngineEditor
{

	struct IMGuiRender
	{
		_Render::DescriptorPool DescriptorPool;
		_Render::RenderPass Renderpass;
		std::vector<_Render::FrameBuffer> FrameBuffers;

		Callback NewFrame;
		Callback DrawFrame;
		Callback SwapChainRebuild;
		Callback OnRenderDestroyed;

		bool FontInitialized;
	};

	void IMGuiRender_init(IMGuiRender* p_imguiTest, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void IMGuiRender_free(IMGuiRender* p_imguiTest, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
}
