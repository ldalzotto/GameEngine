#pragma once

#include <vector>

#include "vulkan/vulkan.h"
#include "Render/Mesh/Mesh.h"

namespace _GameEngine::_Render
{
	struct MeshDrawStep
	{
		std::vector<Mesh*> MeshedToDraw;
	};

	void MeshDrawStep_buildCommandBuffer(MeshDrawStep* p_meshDrawStep, VkCommandBuffer p_commandBuffer);
}