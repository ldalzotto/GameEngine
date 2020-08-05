#pragma once

#include "Includes/VulkanIncludes.h"

#include <vector>

#include "Functional/Callback/CallbackT.hpp"

#include "VulkanObjects/GraphcisPipeline/RenderPass.h"
#include "VulkanObjects/GraphcisPipeline/FrameBuffer.h"
#include "VulkanObjects/Descriptor/DescriptorPool.h"

using namespace _GameEngine;

namespace _GameEngine
{
	struct GameEngineApplicationInterface;

	namespace _Render
	{
		struct BeforeEndRecordingMainCommandBuffer_Input;
		struct RenderInterface;
	}
}

namespace _GameEngineEditor
{

	struct IMGuiRender
	{
		_Render::DescriptorPool DescriptorPool;
		_Render::RenderPass Renderpass;
		std::vector<_Render::FrameBuffer> FrameBuffers;

		_Core::CallbackT<IMGuiRender, GameEngineApplicationInterface> NewFrame;
		_Core::CallbackT<IMGuiRender, _Render::BeforeEndRecordingMainCommandBuffer_Input> DrawFrame;
		_Core::CallbackT<IMGuiRender, _Render::RenderInterface> SwapChainRebuild;

		bool FontInitialized;
	};

	void IMGuiRender_init(IMGuiRender* p_imguiTest, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void IMGuiRender_free(IMGuiRender* p_imguiTest, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
}
