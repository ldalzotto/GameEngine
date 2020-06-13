#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct RenderInterface;
}

namespace _GameEngine::_Render
{
	void DefaultMaterialDrawStep_buildCommandBuffer(RenderInterface* p_renderInterface, VkCommandBuffer p_commandBuffer, size_t l_imageIndex);
}