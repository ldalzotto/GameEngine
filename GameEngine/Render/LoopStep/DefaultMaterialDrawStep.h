#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct SwapChain;
	struct Mesh;
	struct DefaultMaterialV2;
	struct MaterialInstance;
}

namespace _GameEngine::_Render
{

	struct DefaultMaterialDrawStep
	{
		DefaultMaterialV2* DefaultMaterial;
		std::vector<MaterialInstance*> MaterialInstances;
	};

	void DefaultMaterialDrawStep_init(DefaultMaterialDrawStep* p_meshDrawStep, DefaultMaterialV2* p_defaultMaterial);
	void DefaultMaterialDrawStep_clear(DefaultMaterialDrawStep* p_meshDrawStep);

	void DefaultMaterialDrawStep_buildCommandBuffer(SwapChain* p_swapChain, DefaultMaterialDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex);
}