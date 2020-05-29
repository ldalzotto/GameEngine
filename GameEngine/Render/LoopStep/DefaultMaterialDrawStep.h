#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "vulkan/vulkan.h"
#include "Render/Materials/DefaultMaterial.h"

namespace _GameEngine::_Render
{
	struct SwapChain;
	struct Mesh;
	struct DefaultMaterial;
}

namespace _GameEngine::_Render
{
	struct DefaultMaterialDrawCommand
	{
		Mesh* Mesh;
		DefaultMaterialInstance DefaultMaterialInstance;
	};

	struct DefaultMaterialDrawStep
	{
		DefaultMaterial* DefaultMaterial;
		std::vector<DefaultMaterialDrawCommand*> MeshDrawCommands;
	};

	void DefaultMaterialDrawStep_init(DefaultMaterialDrawStep* p_meshDrawStep, DefaultMaterial* p_defaultMaterial);
	void DefaultMaterialDrawStep_clear(DefaultMaterialDrawStep* p_meshDrawStep);

	void DefaultMaterialDrawStep_buildCommandBuffer(SwapChain* p_swapChain, DefaultMaterialDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex);
}