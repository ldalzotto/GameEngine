#pragma once

#include "vulkan/vulkan.h"

#include <vector>

#include "VulkanObjects/GraphcisPipeline/RenderPass.h"
#include "VulkanObjects/GraphcisPipeline/FrameBuffer.h"

#include "VulkanObjects/Descriptor/DescriptorPool.h"

using namespace _GameEngine;

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngineEditor
{
	struct IMGuiRender
	{
		_Render::DescriptorPool DescriptorPool;
		_Render::RenderPass Renderpass;
		std::vector<_Render::FrameBuffer> FrameBuffers;

		bool FontInitialized;
	};

	void IMGuiRender_init(IMGuiRender* p_imguiTest, _Render::RenderInterface* p_renderInterface);
	void IMGuiRender_free(IMGuiRender* p_imguiTest, _Render::RenderInterface* p_renderInterface);
	void IMGuiRender_newFrame(IMGuiRender* p_imguiTest, _Render::RenderInterface* p_renderInterface);
	void IMGuiRender_drawFrame(IMGuiRender* p_imguiTest, VkCommandBuffer p_commandBuffer, size_t l_imageIndex, _Render::RenderInterface* p_renderInterface);
	void IMGuiRender_onSwapChainRebuilded(IMGuiRender* p_imguiTest, _Render::RenderInterface* p_renderInterface);
}
