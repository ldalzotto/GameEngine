#pragma once

#include <vector>

#include "vulkan/vulkan.h"
#include "Render/Device/Device.h"

namespace _GameEngine::_Render::_Semaphore
{
	struct RenderSemaphoreDependencies
	{
		_Device::Device* Device;
	};

	struct RenderSemaphoreCreationInfo
	{
		RenderSemaphoreDependencies* RenderSemaphoreDependencies;
		int MaxFramesInParallel;
	};

	struct RenderSemaphore
	{
		RenderSemaphoreDependencies RenderSemaphoreDependencies;
		int MaxFramesInParallel;
		size_t CurrentFrame;
		std::vector<VkSemaphore> ImageAvailableSemaphores;
		std::vector<VkSemaphore> RenderFinishedSemaphores;
		std::vector<VkFence> WaitForGraphicsQueueFences;
	};

	void RenderSemaphore_init(RenderSemaphore* p_renderSemaphore, RenderSemaphoreCreationInfo* p_renderSemaphoreCreationInfo);
	void RenderSemaphore_free(RenderSemaphore* p_renderSemaphore);

	void RenderSemaphore_incrementFrameCount(RenderSemaphore* p_renderSemaphore);

	struct CurrentSynchronisationObject
	{
		VkSemaphore ImageAvailableSemaphore;
		VkSemaphore RenderFinishedSemaphore;
		VkFence WaitForGraphicsQueueFence;
	};

	CurrentSynchronisationObject RenderSemaphore_getCurrentSynchronisationObject(RenderSemaphore* p_renderSemaphore);
};