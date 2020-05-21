#pragma once

#include <vulkan/vulkan.h>

#include <cstdint>
#include <functional>

namespace _GameEngine::_Render::_Device
{
	/**
		@brief QueueFamily are handles of vulkan queues associated to a @ref PhysicalDevice
	*/
	struct QueueFamily
	{
		bool QueueFound;
		uint32_t QueueIndex;
	};

	struct QueueFamilies
	{
		/**
			@brief Queue that execute all draw operations.
		*/
		QueueFamily Graphics;

		/**
			@brief Queue that display the final image on screen.
		*/
		QueueFamily Present;
	};

	bool getQueue(VkPhysicalDevice p_physicalDevice, 
			QueueFamilies* out_queueFamilies,
			std::function<void(VkPhysicalDevice, uint32_t p_queueFamilyIndex, VkBool32*)> p_surfaceSupport);
}

