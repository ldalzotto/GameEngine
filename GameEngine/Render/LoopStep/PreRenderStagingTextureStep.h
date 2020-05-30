#pragma once

#include "vulkan/vulkan.h"
#include "Render/Memory/VulkanBuffer.h"

namespace _GameEngine::_Render
{

	struct PrerenderStaggingStagingOperationCompleted
	{
		void(*OnStaggingDone)(void*);
		void* Closure;
	};

	struct PreRenderStagingTextureOperation
	{
		VulkanBuffer StagingBuffer;
		VulkanBuffer* TargetBuffer;
		bool StagingOperationCancelled;
		PrerenderStaggingStagingOperationCompleted PrerenderStagingOperationCompleted;
	};

	struct PreRenderStagingTextureStep
	{

	};
}