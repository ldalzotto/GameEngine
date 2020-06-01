#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct SwapChain;
	struct Mesh;
	struct DefaultMaterialV2;
	struct DefaultMaterialV2Instance;
}

namespace _GameEngine::_Render
{

	struct DefaultMaterialDrawStep
	{
		DefaultMaterialV2* DefaultMaterial;
		std::vector<DefaultMaterialV2Instance*> DefaultMaterialV2Instance;
	};

	void DefaultMaterialDrawStep_init(DefaultMaterialDrawStep* p_meshDrawStep, DefaultMaterialV2* p_defaultMaterial);
	void DefaultMaterialDrawStep_clear(DefaultMaterialDrawStep* p_meshDrawStep);

	void DefaultMaterialDrawStep_buildCommandBuffer(SwapChain* p_swapChain, DefaultMaterialDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex);
}