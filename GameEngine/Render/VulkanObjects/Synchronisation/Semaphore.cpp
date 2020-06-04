#include "Semaphore.h"

#include "VulkanObjects/Hardware/Device/Device.h"
#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render
{
	void RenderSemaphore_init(RenderSemaphore* p_renderSemaphore, RenderSemaphoreCreationInfo* p_renderSemaphoreCreationInfo)
	{
		p_renderSemaphore->RenderSemaphoreDependencies = *p_renderSemaphoreCreationInfo->RenderSemaphoreDependencies;
		p_renderSemaphore->MaxFramesInParallel = p_renderSemaphoreCreationInfo->MaxFramesInParallel;
		p_renderSemaphore->CurrentFrame = 0;

		p_renderSemaphore->RenderFinishedSemaphores.resize(p_renderSemaphore->MaxFramesInParallel);
		p_renderSemaphore->ImageAvailableSemaphores.resize(p_renderSemaphore->MaxFramesInParallel);
		p_renderSemaphore->WaitForGraphicsQueueFences.resize(p_renderSemaphore->MaxFramesInParallel);

		for (size_t i = 0; i < p_renderSemaphore->MaxFramesInParallel; i++)
		{
			VkSemaphoreCreateInfo l_semaphoreCreateInfo{};
			l_semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(p_renderSemaphore->RenderSemaphoreDependencies.Device->LogicalDevice.LogicalDevice, &l_semaphoreCreateInfo, nullptr, &p_renderSemaphore->ImageAvailableSemaphores[i]) != VK_SUCCESS
				|| vkCreateSemaphore(p_renderSemaphore->RenderSemaphoreDependencies.Device->LogicalDevice.LogicalDevice, &l_semaphoreCreateInfo, nullptr, &p_renderSemaphore->RenderFinishedSemaphores[i]) != VK_SUCCESS
				)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create semaphores!"));
			}

			VkFenceCreateInfo l_waitForEndOfRenderFenceCreationInfo{};
			l_waitForEndOfRenderFenceCreationInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			if (vkCreateFence(p_renderSemaphore->RenderSemaphoreDependencies.Device->LogicalDevice.LogicalDevice,
				&l_waitForEndOfRenderFenceCreationInfo, nullptr, &p_renderSemaphore->WaitForGraphicsQueueFences[i]) != VK_SUCCESS)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create fence!"));
			}
		}

	};

	void RenderSemaphore_free(RenderSemaphore* p_renderSemaphore)
	{
		for (size_t i = 0; i < p_renderSemaphore->MaxFramesInParallel; i++)
		{
			vkDestroySemaphore(p_renderSemaphore->RenderSemaphoreDependencies.Device->LogicalDevice.LogicalDevice, p_renderSemaphore->ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(p_renderSemaphore->RenderSemaphoreDependencies.Device->LogicalDevice.LogicalDevice, p_renderSemaphore->RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(p_renderSemaphore->RenderSemaphoreDependencies.Device->LogicalDevice.LogicalDevice, p_renderSemaphore->WaitForGraphicsQueueFences[i], nullptr);
		}
	};

	void RenderSemaphore_incrementFrameCount(RenderSemaphore* p_renderSemaphore)
	{
		p_renderSemaphore->CurrentFrame = (p_renderSemaphore->CurrentFrame + 1) % p_renderSemaphore->MaxFramesInParallel;
	};

	CurrentSynchronisationObject RenderSemaphore_getCurrentSynchronisationObject(RenderSemaphore* p_renderSemaphore)
	{
		CurrentSynchronisationObject l_currentSynchronisationObject{};
		l_currentSynchronisationObject.ImageAvailableSemaphore = p_renderSemaphore->ImageAvailableSemaphores[p_renderSemaphore->CurrentFrame];
		l_currentSynchronisationObject.RenderFinishedSemaphore = p_renderSemaphore->RenderFinishedSemaphores[p_renderSemaphore->CurrentFrame];
		l_currentSynchronisationObject.WaitForGraphicsQueueFence = p_renderSemaphore->WaitForGraphicsQueueFences[p_renderSemaphore->CurrentFrame];
		return l_currentSynchronisationObject;
	};
};