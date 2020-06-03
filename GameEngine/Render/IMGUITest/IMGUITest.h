#pragma once

#include "vulkan/vulkan.h"

#include <vector>

#include "Render/GraphcisPipeline/RenderPass.h"
#include "Render/GraphcisPipeline/FrameBuffer.h"

#include "Render/Descriptor/DescriptorPool.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	struct IMGUITest
	{
		DescriptorPool DescriptorPool;
		RenderPass Renderpass;
		std::vector<FrameBuffer> FrameBuffers;

		bool FontInitialized;
	};

	void IMGUITest_init(IMGUITest* p_imguiTest, RenderInterface* p_renderInterface);
	void IMGUITest_free(IMGUITest* p_imguiTest, RenderInterface* p_renderInterface);
	void createFinalDrawObjects(_GameEngine::_Render::RenderInterface* p_renderInterface, _GameEngine::_Render::IMGUITest* p_imguiTest);
	void IMGUITest_drawFrame(IMGUITest* p_imguiTest, VkCommandBuffer p_commandBuffer, size_t l_imageIndex, RenderInterface* p_renderInterface);
	void IMGUITest_onSwapChainRebuilded(IMGUITest* p_imguiTest, RenderInterface* p_renderInterface);
}