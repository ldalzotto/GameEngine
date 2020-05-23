#pragma once

#include <vector>

#include "vulkan/vulkan.h"
#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

namespace _GameEngine::_Render::_Synchronisation
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

	/**
		This @ref RenderSemaphore is used by the render loop because we want to be sure
		that the CPU is not faster than GPU. Thus, we wait for the end of the rendering before starting another one.
	*/
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