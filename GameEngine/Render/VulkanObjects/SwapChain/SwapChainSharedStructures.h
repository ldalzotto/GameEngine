#pragma once

#include "Includes/VulkanIncludes.h"

namespace _GameEngine::_Render
{
	struct SwapChainInfo
	{
		VkExtent2D SwapExtend;
		VkSurfaceFormatKHR SurfaceFormat;
		VkPresentModeKHR PresentMode;
	};

}