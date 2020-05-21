#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render::_SwapChain
{
	struct SwapChainInfo
	{
		VkExtent2D SwapExtend;
		VkSurfaceFormatKHR SurfaceFormat;
		VkPresentModeKHR PresentMode;
	};

}