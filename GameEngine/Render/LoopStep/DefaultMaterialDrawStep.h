#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "vulkan/vulkan.h"
#include "Render/Materials/DefaultMaterial.h"

namespace _GameEngine::_Render
{
	struct Render;
	struct Mesh;
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

	void DefaultMaterialDrawStep_init(DefaultMaterialDrawStep* p_meshDrawStep, Render* p_render);
	void DefaultMaterialDrawStep_clear(DefaultMaterialDrawStep* p_meshDrawStep, Render* p_render);

	void DefaultMaterialDrawStep_buildCommandBuffer(Render* p_render, DefaultMaterialDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer, size_t l_imageIndex);
}