
#include <vector>

#include "Render/CommandBuffer/CommandPool.h"
#include "Render/GraphcisPipeline/FrameBuffer.h"

namespace _GameEngine::_Render::_CommandBuffer
{

	struct CommandBuffersDependencies
	{
		CommandPool* CommandPool;
		_GraphicsPipeline::FrameBuffer* FrameBuffer;
	};

	struct CommandBuffers
	{
		CommandBuffersDependencies CommandBuffersDependencies;
		std::vector<VkCommandBuffer> CommandBuffers;
	};

	void CommandBuffers_init(CommandBuffers* p_commandBuffers, CommandBuffersDependencies* p_commandBuffersDependencies);


	struct GetFrameBufferInfo
	{
		size_t CommandBufferIndex;
		CommandBuffers* CommandBuffers;
	};

	VkFramebuffer get_frameBuffer(GetFrameBufferInfo* p_getFrameBufferInfo);
};