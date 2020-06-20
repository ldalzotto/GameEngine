#pragma once

#include "vulkan/vulkan.h"

#include <vector>

#include "GameEngineApplicationInterface.h"
#include "Utils/Observer/Observer.h"

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

		_Utils::Subject NewFrame;
		_Utils::Subject DrawFrame;
		_Utils::Subject SwapChainRebuild;
		_Utils::Subject OnRenderDestroyed;

		bool FontInitialized;
	};

	void IMGuiRender_init(IMGuiRender* p_imguiTest, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void IMGuiRender_free(IMGuiRender* p_imguiTest, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
}
